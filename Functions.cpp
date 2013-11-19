#include "common.h"

COLORREF CALLBACK CallBlitProc(COLORREF dest, COLORREF src, DWORD lParam)
{
	LPRDATA rdPtr = (LPRDATA)lParam;
	rdPtr->colDest = dest;
	rdPtr->colSrc = src;
	rdPtr->colRet = false;
	rdPtr->rRd->GenerateEvent(8);
	return rdPtr->colNew;
}

//COLORREF CALLBACK AlphaBlitProc(COLORREF dest, COLORREF src, DWORD lParam)
//{
//	float d = dest/float(0xffffff);
//	float s = src/float(0xffffff);
//	return int((s+d*(1-s))*0xffffff);
//}

COLORREF CALLBACK OpBlitProc(COLORREF dest, COLORREF src, DWORD lParam)
{
	LPRDATA rdPtr = (LPRDATA)lParam;

	BYTE* srcBytes = (BYTE*)&src;
	BYTE* destBytes = (BYTE*)&dest;

	char* op = rdPtr->bProcOp;

	float cr = srcBytes[0]*rdPtr->bProcOpSrc;
	float cg = srcBytes[1]*rdPtr->bProcOpSrc;
	float cb = srcBytes[2]*rdPtr->bProcOpSrc;

	//Hardcode *, **, /
	if(*op=='*'||*op=='/')
	{
		cr /= 255.0f;
		cg /= 255.0f;
		cb /= 255.0f;
	}
	//Prevent division by zero
	if(*op=='/'||*op=='%')
	{
		if(!cr) cr = 0.001f;
		if(!cg) cg = 0.001f;
		if(!cb) cb = 0.001f;
	}

	Operation(op,destBytes,cr);
	Operation(op,destBytes+1,cg);
	Operation(op,destBytes+2,cb);

	return dest;
}

void RotatePoint(double angle, int* hotX, int* hotY, int sw, int sh, int* dx, int* dy)
{
	//Rotate hotspot
	float hx = (float)*hotX;
	float hy = (float)*hotY;

	float si = sin(angle);
	float co = cos(angle);

	int trX = sw*co;
	int trY = -sw*si;
	int blX = sh*si;
	int blY = sh*co;
	int brX = sw*co + sh*si;
	int brY = sh*co - sw*si;

	//Update hotspot
	*hotX = round(hx*co + hy*si);
	*hotY = round(hy*co - hx*si);

	//Update topleft coordinate
	*dx += min(0,min(trX,min(blX,brX)));
	*dy += min(0,min(trY,min(blY,brY)));
}

bool Blit(cSurface* source,cSurface* dest,LPRDATA rdPtr)
{
	//Some quick checks
	if(rdPtr->bsRegion && (rdPtr->bsW <= 0 || rdPtr->bsH <= 0))
		return false;
	if(rdPtr->bStretch && (rdPtr->bdW <= 0 || rdPtr->bdH <= 0))
		return false;

	//Some important variables
	int dx = rdPtr->bdX;
	int dy = rdPtr->bdY;
	int sx = rdPtr->bsRegion ? rdPtr->bsX : 0;
	int sy = rdPtr->bsRegion ? rdPtr->bsY : 0;
	int sw = source->GetWidth();
	int sh = source->GetHeight();

	//Prepare hot spot
	int hotX = rdPtr->bhX;
	int hotY = rdPtr->bhY;
	if(rdPtr->bhMode & 2)
	{
		hotX = sw/100.0*hotX;
		hotY = sh/100.0*hotY;
	}

	//Use absolute coords
	if(rdPtr->useAbs)
	{
		if(TargetImg==source)
		{
			sx -= rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;
			sy -= rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
		}
		if(TargetImg==dest)
		{
			dx -= rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;
			dy -= rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
		}
	}

	// stretch flag (used by multiple branches)
	int strf = 0;
	//Resampling
	if(rdPtr->bStretch==2) strf |= STRF_RESAMPLE;
	else if(rdPtr->bStretch==3) strf |= STRF_RESAMPLE_TRANSP;


#ifdef HWABETA
	/* HWA */
	if (source->GetType() >= ST_HWA_RTTEXTURE && dest->GetType() >= ST_HWA_SCREEN)
	{
		// Use region if necessary
		if(rdPtr->bsRegion)
		{
			sx = max(0,min(sw,sx));
			sy = max(0,min(sh,sy));
			sw = max(0,min(rdPtr->bsW,sw));
			sh = max(0,min(rdPtr->bsH,sh));
		}

		if(!sw || !sh)
			return false;

		POINT hotspot = {0, 0};

		if(rdPtr->bhMode)
		{
			hotspot.x = hotX;
			hotspot.y = hotY;
		}
		
		float scaleX = 1;
		float scaleY = 1;

		if(rdPtr->bStretch)
		{
			scaleX = (rdPtr->bdW * 1.0f) / sw;
			scaleY = (rdPtr->bdH * 1.0f) / sh;
		}

		float angle = rdPtr->bAngle;

		return source->BlitEx(*dest, dx, dy, scaleX, scaleY, sx, sy, sw, sh, &hotspot, angle, rdPtr->bM, rdPtr->bOp, rdPtr->bParam, strf);
	}
#endif

	//Compose alpha
	if(rdPtr->bAlphaCompose && rdPtr->bM == BMODE_TRANSP)
	{

		//Get dimensions
		int dw = dest->GetWidth();
		int dh = dest->GetHeight();

		//Create alpha channel if necessary
		if(!source->HasAlpha())
		{
			COLORREF transp = source->GetTransparentColor();
			source->CreateAlpha();
			cSurface* alpha = source->GetAlphaSurface();
			for(int x=0;x<sw;x++)
				for(int y=0;y<sh;y++)
					if(source->GetPixelFast(x,y)!=transp)
						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
			source->ReleaseAlphaSurface(alpha);
		}
		if(!dest->HasAlpha())
		{
			COLORREF transp = dest->GetTransparentColor();
			dest->CreateAlpha();
			cSurface* alpha = dest->GetAlphaSurface();
			for(int x=0;x<dw;x++)
				for(int y=0;y<dh;y++)
					if(dest->GetPixelFast(x,y)!=transp)
						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
			dest->ReleaseAlphaSurface(alpha);
		}

		//Source region or not
		int x1 = sx;
		int y1 = sy;
		int x2 = rdPtr->bsRegion ? (x1+rdPtr->bsW) : sw;
		int y2 = rdPtr->bsRegion ? (y1+rdPtr->bsH) : sh;

		x1 = max(0,min(sw,x1));
		y1 = max(0,min(sh,y1));
		x2 = max(0,min(sw,x2));
		y2 = max(0,min(sh,y2));

		//Nothing to do
		if(x1 >= x2 || y1 >= y2)
			return false;
		
		//Scale if necessary
		cSurface scaled;
		if(rdPtr->bStretch)
		{
			scaled.Create(rdPtr->bdW,rdPtr->bdH,source);
			scaled.SetTransparentColor(source->GetTransparentColor());
			source->Stretch(scaled,0,0,rdPtr->bdW,rdPtr->bdH,x1,y1,x2-x1,y2-y1,BMODE_OPAQUE,BOP_COPY,0,STRF_COPYALPHA|(rdPtr->bStretch-1));
			source = &scaled;
			dx += x1;
			dy += y1;
			hotX *= rdPtr->bdW*1.0f/sw; //(x2-x1) makes hotspot relative to region
			hotY *= rdPtr->bdH*1.0f/sh; //(y2-y1)
			sw = rdPtr->bdW;
			sh = rdPtr->bdH;
			x1 = 0;
			y1 = 0;
			x2 = sw;
			y2 = sh;
		}

		//Nothing to do now
		if(x1 >= x2 || y1 >= y2)
			return false;

		//Rotation is necessary
		cSurface rotated;
		if(rdPtr->bAngle != 0)
		{
			double angle = rdPtr->bAngle/180.0*PI;
			cSurface region;
			region.Create(x2-x1,y2-y1,source);
			source->Blit(region,-x1,-y1,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
			region.CreateRotatedSurface(rotated,rdPtr->bAngle,rdPtr->bAngleResample,source->GetTransparentColor(),FALSE);
			source = &rotated;
			//Rotate the hotspot and adjust the rotated bounding box
			dx += x1;
			dy += y1;
			if(rdPtr->bhMode & 1)
				RotatePoint(angle,&hotX,&hotY,region.GetWidth(),region.GetHeight(),&dx,&dy);
			sw = rotated.GetWidth();
			sh = rotated.GetHeight();
			x1 = 0;
			y1 = 0;
			x2 = sw;
			y2 = sh;
		}

		//Apply hot spot
		if(rdPtr->bhMode & 1)
		{
			dx -= hotX;
			dy -= hotY;
		}

		cSurface* sourceAlpha = source->GetAlphaSurface();
		if(!sourceAlpha || !sourceAlpha->IsValid()) return false;
		LPBYTE sourceBuff = source->LockBuffer();
		LPBYTE sourceAlphaBuff = sourceAlpha->LockBuffer();
		cSurface* destAlpha = dest->GetAlphaSurface();
		if(!destAlpha || !destAlpha->IsValid())
		{
			if(sourceBuff) source->UnlockBuffer(sourceBuff);
			if(sourceAlphaBuff) source->UnlockAlpha();
			return false;
		}
		LPBYTE destBuff = dest->LockBuffer();
		LPBYTE destAlphaBuff = destAlpha->LockBuffer();

		int pitch = source->GetPitch();
		int alphaPitch = source->GetAlphaPitch();

		//Clip destination
		x1 = max(sx-dx,min(dw-dx+sx,x1));
		y1 = max(sy-dy,min(dh-dy+sy,y1));
		x2 = max(sx-dx,min(dw-dx+sx,x2));
		y2 = max(sy-dy,min(dh-dy+sy,y2));

		//Nothing to do now
		if(x1 >= x2 || y1 >= y2)
			return false;

		float factor = 1.0f;
		//Use user callback
		if(rdPtr->bCallback)
			rdPtr->callback = rdPtr->bCallback;
		//Use semi-transparency
		else if(rdPtr->bOp == BOP_BLEND)
			factor -= rdPtr->bParam/128.0f;

		for(int x=x1; x<x2; ++x)
		{
			int nx = x-sx+dx;

			for(int y=y1; y<y2; ++y)
			{
				int ny = y-sy+dy;

				//Get color & alpha
				COLORREF sc = source->GetPixelFast(x,y);
				int sr = GetRValue(sc);
				int sg = GetGValue(sc);
				int sb = GetBValue(sc);
				float sa = sourceAlpha->GetPixelFast8(x,y)/255.0f*factor;
				COLORREF dc = dest->GetPixelFast(nx,ny);
				int dr = GetRValue(dc);
				int dg = GetGValue(dc);
				int db = GetBValue(dc);
				float da = destAlpha->GetPixelFast8(nx,ny)/255.0f;

				//Output
				float a;
				COLORREF c;

				//Composite (with ink effect)
				if(!rdPtr->bCallback)
				{
					
					switch(rdPtr->bOp)
					{
						case BOP_ADD:
							sr = max(0,min(255,sr+dr));
							sg = max(0,min(255,sg+dg));
							sb = max(0,min(255,sb+db));
							break;
						case BOP_SUB:
							sr = max(0,min(255,dr-sr));
							sg = max(0,min(255,dg-sg));
							sb = max(0,min(255,db-sb));
							break;
						case BOP_INVERT:
							sr = 255-sr;
							sg = 255-sg;
							sb = 255-sb;
							break;
						case BOP_MONO:
							sr = (sr+sg+sb)/3;
							sg = sr;
							sb = sr;
							break;
						case BOP_XOR:
							sr = dr;
							sg = dg;
							sb = db;
							break;
						case BOP_AND:
							sr &= dr;
							sg &= dg;
							sb &= db;
							break;
						case BOP_OR:
							sr |= dr;
							sg |= dg;
							sb |= db;
							break;
					}

					//Result alpha
					a = sa+da*(1-sa);
					//Result color
					int r = (sr*sa+dr*da*(1-sa))/a;
					int g = (sg*sa+dg*da*(1-sa))/a;
					int b = (sb*sa+db*da*(1-sa))/a;
					c = RGB(r,g,b);
				}
				//User callback
				else
				{
					rdPtr->colAlphaSrc = sa*255.0f;
					rdPtr->colAlphaDest = da;
					rdPtr->colSrc = sc;
					rdPtr->colDest = dc;
					rdPtr->callX = nx;
					rdPtr->callY = ny;
					rdPtr->colAlphaRet = false;
					rdPtr->colRet = false;
					rdPtr->rRd->GenerateEvent(8);
					if(rdPtr->colAlphaRet)
						a = rdPtr->colAlphaNew/255.0f;
					else
						a = da;
					if(rdPtr->colRet)
						c = rdPtr->colNew;
					else
						c = dc;
				}

				//Set pixels
				dest->SetPixelFast(nx,ny,c);
				destAlpha->SetPixelFast8(nx,ny,a*255);
			}
		}

		source->UnlockBuffer(sourceBuff);
		sourceAlpha->UnlockBuffer(sourceAlphaBuff);
		source->ReleaseAlphaSurface(sourceAlpha);
		dest->UnlockBuffer(destBuff);
		destAlpha->UnlockBuffer(destAlphaBuff);
		dest->ReleaseAlphaSurface(destAlpha);

		return true;
	}
	//End of alpha composition

	//For hotspots
	int dw = sw;
	int dh = sh;
	if(rdPtr->bStretch)
	{
		dw = rdPtr->bdW;
		dh = rdPtr->bdH;
	}
	if(dw <= 0 || dh <= 0)
		return false;

	//Recursive call for rotated images
	if(rdPtr->bAngle != 0)
	{
		//Create a fake rdPtr with destination (0,0) and angle 0
		RUNDATA fakePtr;
		memcpy((void*)&fakePtr,rdPtr,sizeof(RUNDATA));
		fakePtr.bdX = 0;
		fakePtr.bdY = 0;
		fakePtr.bAngle = 0;
		fakePtr.bCallback = 0;
		fakePtr.bOp = BOP_COPY;
		fakePtr.bhMode = 0;
		//Blit our source image onto a temporary image
		cSurface rotsource;
		rotsource.Create(dw,dh,source);
		if(Blit(source,&rotsource,&fakePtr))
		{
			//Rotate the image and blit onto the destination image
			cSurface rotated;
			rotsource.CreateRotatedSurface(rotated,rdPtr->bAngle,rdPtr->bAngleResample,source->GetTransparentColor(),FALSE);
			fakePtr.bdX = rdPtr->bdX;
			fakePtr.bdY = rdPtr->bdY;
			fakePtr.bCallback = rdPtr->bCallback;
			fakePtr.bOp = rdPtr->bOp;
			fakePtr.bStretch = 0;
			fakePtr.bsRegion = 0;
			fakePtr.bhMode = 0;
			if(rdPtr->bhMode & 1)
			{
				int dw = sw;
				int dh = sh;
				if(rdPtr->bStretch)
				{
					dw = rdPtr->bdW;
					dh = rdPtr->bdH;
					hotX *= dw*1.0f/sw;
					hotY *= dh*1.0f/sh;
				}
				if(rdPtr->bAngle != 0)
					RotatePoint(rdPtr->bAngle/180.0*PI,&hotX,&hotY,dw,dh,&fakePtr.bdX,&fakePtr.bdY);
				fakePtr.bdX -= hotX;
				fakePtr.bdY -= hotY;
			}
			return Blit(&rotated,dest,&fakePtr);
		}
		return false;
	}

	//No rotation, but apply hotspot
	if(rdPtr->bhMode & 1)
	{
		dx -= hotX*dw*1.0f/sw;
		dy -= hotY*dh*1.0f/sh;
	}

	//Use region if necessary
	if(rdPtr->bsRegion)
	{
		sx = max(0,min(sw,sx));
		sy = max(0,min(sh,sy));
		sw = max(0,min(rdPtr->bsW,sw));
		sh = max(0,min(rdPtr->bsH,sh));
	}
	//Just to make sure...
	else
	{
		sx = 0;
		sy = 0;
		sw = source->GetWidth();
		sh = source->GetHeight();
	}

	//Blitting
	if(!rdPtr->bStretch)
	{
		//Filter callback blit
		if(rdPtr->bCallback)
		{
			rdPtr->callback = rdPtr->bCallback;
			return source->FilterBlit(*dest,dx,dy,sx,sy,sw,sh,rdPtr->bM,CallBlitProc,(LPARAM)rdPtr);
		}

		//Operation blit
		if(rdPtr->bProcOp)
			return source->FilterBlit(*dest,dx,dy,sx,sy,sw,sh,rdPtr->bM,OpBlitProc,(LPARAM)rdPtr);

		//Simple blit
		return source->Blit(*dest,dx,dy,sx,sy,sw,sh,
			rdPtr->bM,rdPtr->bOp,rdPtr->bParam,rdPtr->bFlags);
	}
	//Stretching
	else
	{
		//Copy alpha
		if(rdPtr->bFlags & BLTF_COPYALPHA)
			strf |= STRF_COPYALPHA;
		//Stretch
		return source->Stretch(*dest,dx,dy,rdPtr->bdW,rdPtr->bdH,sx,sy,sw,sh,
			rdPtr->bM,rdPtr->bOp,rdPtr->bParam,strf);
	}
}

void FreeColMask(LPRDATA rdPtr)
{
	if (rdPtr->collision)
	{
		free(rdPtr->collision);
		rdPtr->collision = 0;
	}
}

void UpdateHotspot(LPRDATA rdPtr)
{
	if(rdPtr->currentId < 0 || !CurrentImg)
		return;

	//Get scale (absolute since negative will mirror)
	float scaleX = abs(rdPtr->rc.rcScaleX);
	float scaleY = abs(rdPtr->rc.rcScaleY);

	//Get scaled size
	int width = CurrentImg->GetWidth()*scaleX;
	int height = CurrentImg->GetHeight()*scaleY;

	//Get scaled hotspot
	SImage* si = ImageS(rdPtr->currentId);
	int hotX = si->hotX*scaleX;
	int hotY = si->hotY*scaleY;

	int dx = 0, dy = 0;
	if(rdPtr->rc.rcAngle)
	{
		//Rotate hotspot
		double angle = rdPtr->rc.rcAngle/180.0*PI;
		RotatePoint(angle,&hotX,&hotY,width,height,&dx,&dy);

		//Calculate rotated size
		cSurface::GetSizeOfRotatedRect(&width,&height,rdPtr->rc.rcAngle);
	}

	//Update size and scale image
	rdPtr->rHo.hoImgWidth = width;
	rdPtr->rHo.hoImgHeight = height;

	//Apply hotspot
	rdPtr->rHo.hoImgXSpot = (short)hotX-dx;
	rdPtr->rHo.hoImgYSpot = (short)hotY-dy;
}

int ProcessFillFlags(RUNDATA*rdPtr,string fill,string out,bool&useFill,bool&useOut,FillGuide*&fillData,FillGuide*&outData)
{
	//We want filling
	if(useFill)
	{
		//Fill data must exist if specified
		if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
		{
			fillData = rdPtr->fill->find(fill)->second;
			//Check if image exists
			if(fillData->img)
			{
				if(fillData->param<0||fillData->param>rdPtr->surf->size()-1)
					return 0;
				cSurface* tmp = Image(fillData->param);
				if(!tmp->IsValid())
					return 0;
				if(fillData->type==FILL_MOSAIC)
					((CFillMosaic*)fillData->fill)->SetTile(tmp);
				//Update mosaic tile...
			}
		}
		else
			useFill = false;
	}
	//We want an outline
	if(useOut)
	{
		//Outline data must exist if thickness!=0
		if(rdPtr->fill->find(out)!=rdPtr->fill->end())
		{
			outData = rdPtr->fill->find(out)->second;
			//Check if image exists
			if(outData->img)
			{
				if(outData->param<0||outData->param>rdPtr->fill->size())
					return 0;
				cSurface* tmp = Image(outData->param);
				if(!tmp->IsValid())
					return 0;
				//Update mosaic tile...
				if(outData->type==FILL_MOSAIC)
					((CFillMosaic*)outData->fill)->SetTile(tmp);
			}
		}
		else
			useOut = false;
	}
	return 1;
}

cSurface* GetImgAt(RUNDATA* rdPtr,int image)
{
	if(image<0||image>=rdPtr->surf->size())
		return 0;
	return Image(image);
}

int GetImgCount(RUNDATA* rdPtr)
{
	return rdPtr->surf->size();
}

void Operation(char* op,BYTE* a,float b)
{
	int temp = *a;
	switch(*op)
	{
		case '+': temp += b; break;
		case '-': temp -= b; break;
		case '*':
			{
				if(op[1]=='*')
					temp = pow(temp/255.0f,b)*255;
				else
					temp *= b;
				break;
			}
		case '/': temp /= b; break;
		case '%': temp %= (int)b; break;
		case '<':
				if(op[1]=='<')
					temp <<= (int)b;
				else
					temp = min(temp,b);
				break;
		case '>':
				if(op[1]=='>')
					temp >>= (int)b;
				else
					temp = max(temp,b);
				break;
		case '^': temp ^= (int)b; break;
		case '&': temp &= (int)b; break;
		case '|': temp |= (int)b; break;
		case '=': temp = b; break;
	}
	if(temp > 255)
		temp = 255;
	else if(temp < 0)
		temp = 0;
	//Update
	*a = temp;
}

//#define skipspace() while(isspace(*cond)) cond++
//void ParseCondition(LPRDATA rdPtr, char* cond, ColorCond* conds, int n)
//{
//	int cur = 0;
//	while(*cond)
//	{
//		skipspace();
//		if(strchr("rgba",tolower(*cond)))
//			conds[cur].channel = tolower(*cond++);
//		skipspace();
//		if(strchr("<>=!",*cond))
//		{
//			conds[cur].condition[0] = *cond;
//			if(cond[1] == '=')
//				conds[cur].condition[1] = *++cond;
//		}
//		cond++;
//		skipspace();
//		//Variable parameter
//		if(strchr("rgba",tolower(*cond)))
//		{
//			conds[cur].parameter = 0xff00|tolower(*cond);
//		}
//		//Numeric parameter
//		else
//			conds[cur].parameter = 0xff & strtol((const char*)cond,&cond,10);
//		skipspace();
//		if(*cond++ != ';') break;
//		//Go to next condition
//		if(++cur == n) break;
//	}
//} 
//
//bool ConditionMet(LPRDATA rdPtr, ColorCond* conds, int n, COLORREF color)
//{
//	BYTE* col = (BYTE*)&color;
//
//	for(int i = 0; i < n; ++i)
//	{
//		if(conds[i].channel == 0)
//			break;
//
//		int channel = 0;
//		switch(conds[i].channel)
//		{
//			case 'r': channel = col[0]; break;
//			case 'g': channel = col[1]; break;
//			case 'b': channel = col[2]; break;
//			case 'a': channel = col[3]; break;
//			default: continue;
//		}
//
//		int param = conds[i].parameter;
//		if(param & 0xff00)
//		{
//			switch(param & 0xff)
//			{
//				case 'r': param = col[0]; break;
//				case 'g': param = col[1]; break;
//				case 'b': param = col[2]; break;
//				case 'a': param = col[3]; break;
//			}
//		}
//
//		short c = *(short*)&conds[i].condition[0];
//		switch(c)
//		{
//			case '==': if(channel != param) return false; else continue;
//			case '=>': if(channel < param) return false; else continue;
//			case '=<': if(channel > param) return false; else continue;
//			case '=!': if(channel == param) return false; else continue;
//			case '<': if(channel >= param) return false; else continue;
//			case '>': if(channel <= param) return false; else continue;
//		}
//
//	}
//
//	return true;
//}

WORD CreateImageFromSurface(LPMV pMV, LPSURFACE pSf, int dwWidth, int dwHeight, int hotSpotX, int hotSpotY,     int actionPointX, int actionPointY){
        // Create image
        WORD    wMode = (WORD)(pMV->mvAppMode & SM_MASK);
        if ( pSf->HasAlpha() != NULL ){
                wMode |= (IF_ALPHA << 8);
        }
        WORD newImg = (WORD)AddImage (pMV->mvIdAppli, (WORD)dwWidth, (WORD)dwHeight, hotSpotX, hotSpotY,
                                                                actionPointX, actionPointY, pSf->GetTransparentColor(), wMode, 
                                                                NULL, NULL);
        if ( newImg != 0 ) {
                // Lock new image surface
                cSurface sfNewImg;
                if ( LockImageSurface(pMV->mvIdAppli, newImg, sfNewImg) ) {
                        // Blit surface into image surface
                        pSf->Blit(sfNewImg);
                        // Unlock image surface
                        UnlockImageSurface(sfNewImg);
                } else
                        DelImage(pMV->mvIdAppli, newImg);
        }
        return newImg;
}

//BOOL WINAPI ExportSurface(CImageFilterMgr* pImgMgr, LPCSTR pFileName, cSurface* psf, DWORD dwFilterID, int compress) 
//{ 
//    CImageFilter pFilter(pImgMgr);
//
//
//	if(!dwFilterID) return FALSE;
//
//    if ( psf == NULL || !psf->IsValid() ) 
//        return FALSE; 
//    if ( pFileName == NULL || strlen(pFileName) == 0 ) 
//        return FALSE; 
//    pFilter.UseSpecificFilterID(dwFilterID);
//    if ( !pFilter.CanSave() ) 
//        return FALSE; 
//
//    // Write only 8 or 24 bit surfaces, or 32 bit surfaces if alpha channel 
//    BOOL bHasAlpha = psf->HasAlpha(); 
//    int nWidth = psf->GetWidth(); 
//    int nHeight = psf->GetHeight(); 
//    int nDepth = psf->GetDepth(); 
//    cSurface* pSaveSf = psf; 
//    if (bHasAlpha) 
//        nDepth = 32; 
//    else nDepth = 24; 
//    if (nDepth != psf->GetDepth()) 
//    { 
//        cSurface* pProto = NULL; 
//        GetSurfacePrototype(&pProto,nDepth,ST_MEMORY,SD_DIB); 
//        pSaveSf = new cSurface; 
//        pSaveSf->Create(nWidth,nHeight,pProto);
//        psf->Blit(*pSaveSf); 
//    }
//
//    // Write file 
//    BOOL bOK = FALSE; 
//    LPBYTE pbuf = pSaveSf->LockBuffer(); 
//    if ( pbuf != NULL ) 
//    { 
//        LPBYTE pAlpha = NULL; 
//        int nAlphaPitch = 0; 
//        if ( pSaveSf->HasAlpha() ) 
//        { 
//            pAlpha = pSaveSf->LockAlpha(); 
//            nAlphaPitch = pSaveSf->GetAlphaPitch();	
//        }
//		pFilter.SetCompressionLevel(compress);
//        int error = pFilter.Save(pFileName, pbuf, nWidth, nHeight, nDepth, pSaveSf->GetPitch(), (LPLOGPALETTE)pSaveSf->GetPalette(), pAlpha, nAlphaPitch); 
//        if ( pAlpha ) 
//            pSaveSf->UnlockAlpha(); 
//        pSaveSf->UnlockBuffer(pbuf); 
//
//        // OK? 
//        if ( error == IF_OK ) 
//            bOK = TRUE; 
//    } 
//
//    // Delete temporary surface 
//    if ( pSaveSf != psf ) 
//        delete pSaveSf; 
//
//    return bOK; 
//}

const TCHAR * GetExtension(const TCHAR* Filename)
{
	const TCHAR * Extension = (Filename + _tcslen(Filename)) - 1;
	for(; Extension != Filename && *Extension != '.'; --Extension);
	return ++Extension;
}

DWORD FindFilter(CImageFilterMgr* pImgMgr, TCHAR* file,bool isext)
{
	//Get filter by extension
	const TCHAR* ext = isext ? file : GetExtension(file);
	DWORD filter = 0;
	for(int i=0;i<pImgMgr->GetFilterCount();++i)
	{
		const TCHAR** exts = pImgMgr->GetFilterExts(i);
		if(exts == 0) break;
		for(int j=0;exts[j];j++)
		{
			if(!_tcsicmp(exts[j],ext))
			{
				filter = pImgMgr->GetFilterID(i);
				break;
			}
		}
	} 
	return filter;
}

	////Alpha channels first
	//cSurface newDestAlpha;
	//if(rdPtr->bAlphaCompose && rdPtr->bM == BMODE_TRANSP && (source->HasAlpha() || dest->HasAlpha()))
	//{
	//	cSurface* sourceAlpha = 0;
	//	cSurface* destAlpha = 0;
	//	if(source->HasAlpha())
	//		sourceAlpha = source->GetAlphaSurface();
	//	else
	//	{
	//		int sw = source->GetWidth();
	//		int sh = source->GetHeight();
	//		COLORREF transp = source->GetTransparentColor();
	//		source->CreateAlpha();
	//		sourceAlpha = source->GetAlphaSurface();
	//		for(int x=0;x<sw;x++)
	//			for(int y=0;y<sh;y++)
	//				if(source->GetPixelFast(x,y)!=transp)
	//					sourceAlpha->SetPixel(x,y,255);
	//	}
	//	if(dest->HasAlpha())
	//		destAlpha = dest->GetAlphaSurface();
	//	else
	//	{
	//		int dw = dest->GetWidth();
	//		int dh = dest->GetHeight();
	//		COLORREF transp = dest->GetTransparentColor();
	//		dest->CreateAlpha();
	//		destAlpha = dest->GetAlphaSurface();
	//		for(int x=0;x<dw;x++)
	//			for(int y=0;y<dh;y++)
	//				if(dest->GetPixelFast(x,y)!=transp)
	//					destAlpha->SetPixel(x,y,255);
	//	}

	//	newDestAlpha.Clone(*destAlpha);
	//	sourceAlpha->FilterBlit(newDestAlpha,&AlphaBlitProc,0,BMODE_OPAQUE);

	//	source->ReleaseAlphaSurface(sourceAlpha);
	//	dest->ReleaseAlphaSurface(destAlpha);
	//}

//
//bool Blit(cSurface* source,cSurface* dest,LPRDATA rdPtr)
//{
//	//Some quick checks
//	if(rdPtr->bsRegion && (rdPtr->bsW <= 0 || rdPtr->bsH <= 0))
//		return false;
//	if(rdPtr->bStretch && (rdPtr->bdW <= 0 || rdPtr->bdH <= 0))
//		return false;
//
//	//Some important variables
//	int dx = rdPtr->bdX;
//	int dy = rdPtr->bdY;
//	int sx = rdPtr->bsRegion ? rdPtr->bsX : 0;
//	int sy = rdPtr->bsRegion ? rdPtr->bsY : 0;
//	int sw = source->GetWidth();
//	int sh = source->GetHeight();
//
//	//Use absolute coords
//	if(rdPtr->useAbs)
//	{
//		if(TargetImg==source)
//		{
//			sx -= rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;
//			sy -= rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
//		}
//		if(TargetImg==dest)
//		{
//			dx -= rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;
//			dy -= rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
//		}
//	}
//
//	//Compose alpha
//	if(rdPtr->bAlphaCompose && rdPtr->bM == BMODE_TRANSP)
//	{
//
//		//Get dimensions
//		int dw = dest->GetWidth();
//		int dh = dest->GetHeight();
//
//		//Create alpha channel if necessary
//		if(!source->HasAlpha())
//		{
//			COLORREF transp = source->GetTransparentColor();
//			source->CreateAlpha();
//			cSurface* alpha = source->GetAlphaSurface();
//			for(int x=0;x<sw;x++)
//				for(int y=0;y<sh;y++)
//					if(source->GetPixelFast(x,y)!=transp)
//						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
//			source->ReleaseAlphaSurface(alpha);
//		}
//		if(!dest->HasAlpha())
//		{
//			COLORREF transp = dest->GetTransparentColor();
//			dest->CreateAlpha();
//			cSurface* alpha = dest->GetAlphaSurface();
//			for(int x=0;x<dw;x++)
//				for(int y=0;y<dh;y++)
//					if(dest->GetPixelFast(x,y)!=transp)
//						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
//			dest->ReleaseAlphaSurface(alpha);
//		}
//
//		//Source region or not
//		int x1 = sx;
//		int y1 = sy;
//		int x2 = rdPtr->bsRegion ? (x1+rdPtr->bsW) : sw;
//		int y2 = rdPtr->bsRegion ? (y1+rdPtr->bsH) : sh;
//
//		x1 = max(0,min(sw-1,x1));
//		y1 = max(0,min(sh-1,y1));
//		x2 = max(0,min(sw-1,x2));
//		y2 = max(0,min(sh-1,y2));
//
//		//Nothing to do
//		if(x1 >= x2 || y1 >= y2)
//			return false;
//
//		//Prepare hot spot
//		int hotX = rdPtr->bhX;
//		int hotY = rdPtr->bhY;
//		if(rdPtr->bhMode & 2)
//		{
//			hotX = sw/100.0*hotX;
//			hotY = sh/100.0*hotY;
//		}
//		
//		//Scale if necessary
//		cSurface scaled;
//		if(rdPtr->bStretch)
//		{
//			scaled.Create(rdPtr->bdW,rdPtr->bdH,source);
//			scaled.SetTransparentColor(source->GetTransparentColor());
//			source->Stretch(scaled,0,0,rdPtr->bdW,rdPtr->bdH,x1,y1,x2-x1,y2-y1,BMODE_OPAQUE,BOP_COPY,0,STRF_COPYALPHA|(rdPtr->bStretch-1));
//			source = &scaled;
//			dx += x1;
//			dy += y1;
//			hotX *= rdPtr->bdW*1.0f/sw; //(x2-x1) makes hotspot relative to region
//			hotY *= rdPtr->bdH*1.0f/sh; //(y2-y1)
//			sw = rdPtr->bdW;
//			sh = rdPtr->bdH;
//			x1 = 0;
//			y1 = 0;
//			x2 = sw;
//			y2 = sh;
//		}
//
//		//Nothing to do now
//		if(x1 >= x2 || y1 >= y2)
//			return false;
//
//		//Rotation is necessary
//		cSurface rotated;
//		if(rdPtr->bAngle != 0)
//		{
//			double angle = rdPtr->bAngle/180.0*PI;
//			cSurface region;
//			region.Create(x2-x1,y2-y1,source);
//			source->Blit(region,-x1,-y1,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
//			region.CreateRotatedSurface(rotated,rdPtr->bAngle,rdPtr->bAngleResample,source->GetTransparentColor(),FALSE);
//			source = &rotated;
//			//Rotate the hotspot and adjust the rotated bounding box
//			dx += x1;
//			dy += y1;
//			if(rdPtr->bhMode & 1)
//				RotatePoint(angle,&hotX,&hotY,region.GetWidth(),region.GetHeight(),&dx,&dy);
//			sw = rotated.GetWidth();
//			sh = rotated.GetHeight();
//			x1 = 0;
//			y1 = 0;
//			x2 = sw;
//			y2 = sh;
//		}
//
//		//Apply hot spot
//		if(rdPtr->bhMode & 1)
//		{
//			dx -= hotX;
//			dy -= hotY;
//		}
//
//		//Lock all necessary buffers
//		cSurface* sourceAlpha = source->GetAlphaSurface();
//		if(!sourceAlpha || !sourceAlpha->IsValid()) return false;
//		LPBYTE sourceBuff = source->LockBuffer();
//		LPBYTE sourceAlphaBuff = sourceAlpha->LockBuffer();
//		cSurface* destAlpha = dest->GetAlphaSurface();
//		if(!destAlpha || !destAlpha->IsValid())
//		{
//			if(sourceBuff) source->UnlockBuffer(sourceBuff);
//			if(sourceAlphaBuff) source->UnlockAlpha();
//			return false;
//		}
//		LPBYTE destBuff = dest->LockBuffer();
//		LPBYTE destAlphaBuff = destAlpha->LockBuffer();
//
//		//Get all buffer pitches
//		int sourcePitch = source->GetPitch();
//		if(sourcePitch < 0)
//			sourceBuff -= (sourcePitch*=-1)*(sh-1);
//		int sourceAlphaPitch = source->GetAlphaPitch();
//		if(sourceAlphaPitch < 0)
//			sourceAlphaBuff -= (sourceAlphaPitch*=-1)*(sh-1);
//		int destPitch = dest->GetPitch();
//		if(destPitch < 0)
//			destBuff -= (destPitch*=-1)*(sh-1);
//		int destAlphaPitch = dest->GetAlphaPitch();
//		if(destAlphaPitch < 0)
//			destAlphaBuff -= (destAlphaPitch*=-1)*(dh-1);
//		//Get depths
//		int sourceDepth = source->GetDepth()>>3;
//		int destDepth = dest->GetDepth()>>3;
//
//
//		//Clip destination
//		x1 = max(sx-dx,min(dw-1-dx+sx,x1));
//		y1 = max(sy-dy,min(dh-1-dy+sy,y1));
//		x2 = max(sx-dx,min(dw-1-dx+sx,x2));
//		y2 = max(sy-dy,min(dh-1-dy+sy,y2));
//
//		//Nothing to do now
//		if(x1 >= x2 || y1 >= y2)
//			return false;
//
//		float factor = 1.0f;
//		//Use user callback
//		if(rdPtr->bCallback)
//			rdPtr->callback = rdPtr->bCallback;
//		//Use semi-transparency
//		else if(rdPtr->bOp == BOP_BLEND)
//			factor -= rdPtr->bParam/128.0f;
//
//
//		for(int y=y1; y<y2; ++y)
//		{
//			int ny = y-sy+dy;
//
//			for(int x=x1; x<x2; ++x)
//			{
//				int nx = x-sx+dx;
//
//				BYTE* sourceAddr = sourceBuff+sourcePitch*y+x*sourceDepth;
//				BYTE* sourceAlphaAddr = sourceAlphaBuff+sourceAlphaPitch*y+x;
//				BYTE* destAddr = destBuff+destPitch*ny+nx*destDepth;
//				BYTE* destAlphaAddr = destAlphaBuff+destAlphaPitch*ny+nx;
//
//				//Get color & alpha
//				int sr = sourceAddr[2];
//				int sg = sourceAddr[1];
//				int sb = sourceAddr[0];
//				float sa = sourceAlphaAddr[0]/255.0f*factor;
//				int dr = destAddr[2];
//				int dg = destAddr[1];
//				int db = destAddr[0];
//				float da = destAlphaAddr[0]/255.0f;
//
//				//Output
//				float a;
//				COLORREF c;
//
//				//Composite (with ink effect)
//				if(!rdPtr->bCallback)
//				{
//					
//					switch(rdPtr->bOp)
//					{
//						case BOP_ADD:
//							sr = max(0,min(255,sr+dr));
//							sg = max(0,min(255,sg+dg));
//							sb = max(0,min(255,sb+db));
//							break;
//						case BOP_SUB:
//							sr = max(0,min(255,dr-sr));
//							sg = max(0,min(255,dg-sg));
//							sb = max(0,min(255,db-sb));
//							break;
//						case BOP_INVERT:
//							sr = 255-sr;
//							sg = 255-sg;
//							sb = 255-sb;
//							break;
//						case BOP_MONO:
//							sr = (sr+sg+sb)/3;
//							sg = sr;
//							sb = sr;
//							break;
//						case BOP_XOR:
//							sr = dr;
//							sg = dg;
//							sb = db;
//							break;
//						case BOP_AND:
//							sr &= dr;
//							sg &= dg;
//							sb &= db;
//							break;
//						case BOP_OR:
//							sr |= dr;
//							sg |= dg;
//							sb |= db;
//							break;
//					}
//
//					//Result alpha
//					a = sa+da*(1-sa);
//					//Result color
//					destAddr[2] = (sr*sa+dr*da*(1-sa))/a;
//					destAddr[1] = (sg*sa+dg*da*(1-sa))/a;
//					destAddr[0] = (sb*sa+db*da*(1-sa))/a;
//				}
//				//User callback
//				else
//				{
//					rdPtr->colAlphaSrc = sa*255.0f;
//					rdPtr->colAlphaDest = da;
//					rdPtr->colSrc = RGB(sr,sg,sb);
//					rdPtr->colDest = RGB(dr,dg,db);
//					rdPtr->callX = nx;
//					rdPtr->callY = ny;
//					rdPtr->colAlphaRet = false;
//					rdPtr->colRet = false;
//					rdPtr->rRd->GenerateEvent(8);
//					if(rdPtr->colAlphaRet)
//						a = rdPtr->colAlphaNew/255.0f;
//					else
//						a = da;
//					if(rdPtr->colRet)
//					{
//						destAddr[2] = GetRValue(rdPtr->colNew);
//						destAddr[1] = GetGValue(rdPtr->colNew);
//						destAddr[0] = GetBValue(rdPtr->colNew);
//					}
//				}
//
//				//Se alpha value
//				destAlphaAddr[0] = a*255;
//			}
//		}
//
//		source->UnlockBuffer(sourceBuff);
//		sourceAlpha->UnlockBuffer(sourceAlphaBuff);
//		source->ReleaseAlphaSurface(sourceAlpha);
//		dest->UnlockBuffer(destBuff);
//		destAlpha->UnlockBuffer(destAlphaBuff);
//		dest->ReleaseAlphaSurface(destAlpha);
//
//		return true;
//	}
//	//End of alpha composition
//
//	//For hotspots
//	int dw = sw;
//	int dh = sh;
//	if(rdPtr->bStretch)
//	{
//		dw = rdPtr->bdW;
//		dh = rdPtr->bdH;
//	}
//	if(dw <= 0 || dh <= 0)
//		return false;
//
//	//Recursive call for rotated images
//	if(rdPtr->bAngle != 0)
//	{
//		//Create a fake rdPtr with destination (0,0) and angle 0
//		RUNDATA fakePtr;
//		memcpy((void*)&fakePtr,rdPtr,sizeof(RUNDATA));
//		fakePtr.bdX = 0;
//		fakePtr.bdY = 0;
//		fakePtr.bAngle = 0;
//		fakePtr.bCallback = 0;
//		fakePtr.bOp = BOP_COPY;
//		fakePtr.bhMode = 0;
//		//Blit our source image onto a temporary image
//		cSurface rotsource;
//		rotsource.Create(dw,dh,source);
//		if(Blit(source,&rotsource,&fakePtr))
//		{
//			//Rotate the image and blit onto the destination image
//			cSurface rotated;
//			rotsource.CreateRotatedSurface(rotated,rdPtr->bAngle,rdPtr->bAngleResample,source->GetTransparentColor(),FALSE);
//			fakePtr.bdX = rdPtr->bdX;
//			fakePtr.bdY = rdPtr->bdY;
//			fakePtr.bCallback = rdPtr->bCallback;
//			fakePtr.bOp = rdPtr->bOp;
//			fakePtr.bStretch = 0;
//			fakePtr.bsRegion = 0;
//			fakePtr.bhMode = 0;
//			if(rdPtr->bhMode & 1)
//			{
//				int hotX = rdPtr->bhX;
//				int hotY = rdPtr->bhY;
//				if(rdPtr->bhMode & 2)
//				{
//					hotX = sw/100.0f*hotX;
//					hotY = sh/100.0f*hotY;
//				}
//				int dw = sw;
//				int dh = sh;
//				if(rdPtr->bStretch)
//				{
//					dw = rdPtr->bdW;
//					dh = rdPtr->bdH;
//					hotX *= dw*1.0f/sw;
//					hotY *= dh*1.0f/sh;
//				}
//				if(rdPtr->bAngle != 0)
//					RotatePoint(rdPtr->bAngle/180.0*PI,&hotX,&hotY,dw,dh,&fakePtr.bdX,&fakePtr.bdY);
//				fakePtr.bdX -= hotX;
//				fakePtr.bdY -= hotY;
//			}
//			return Blit(&rotated,dest,&fakePtr);
//		}
//		return false;
//	}
//
//	//No rotation, but apply hotspot
//	if(rdPtr->bhMode & 1)
//	{
//		dx -= rdPtr->bhX*dw*1.0f/sw;
//		dy -= rdPtr->bhY*dh*1.0f/sh;
//	}
//
//	//Use region if necessary
//	if(rdPtr->bsRegion)
//	{
//		sx = max(0,min(sw,sx));
//		sy = max(0,min(sh,sy));
//		sw = max(0,min(rdPtr->bsW,sw));
//		sh = max(0,min(rdPtr->bsH,sh));
//	}
//	//Just to make sure...
//	else
//	{
//		sx = 0;
//		sy = 0;
//		sw = source->GetWidth();
//		sh = source->GetHeight();
//	}
//
//	//Blitting
//	if(!rdPtr->bStretch)
//	{
//		//Filter callback blit
//		if(rdPtr->bCallback)
//		{
//			rdPtr->callback = rdPtr->bCallback;
//			return source->FilterBlit(*dest,dx,dy,sx,sy,sw,sh,rdPtr->bM,CallBlitProc,(LPARAM)rdPtr);
//		}
//
//		//Operation blit
//		if(rdPtr->bProcOp)
//			return source->FilterBlit(*dest,dx,dy,sx,sy,sw,sh,rdPtr->bM,OpBlitProc,(LPARAM)rdPtr);
//
//		//Simple blit
//		return source->Blit(*dest,dx,dy,sx,sy,sw,sh,
//			rdPtr->bM,rdPtr->bOp,rdPtr->bParam,rdPtr->bFlags);
//	}
//	//Stretching
//	else
//	{
//		int strf = 0;
//		//Resampling
//		if(rdPtr->bStretch==2) strf |= STRF_RESAMPLE;
//		else if(rdPtr->bStretch==3) strf |= STRF_RESAMPLE_TRANSP;
//		//Copy alpha
//		if(rdPtr->bFlags==BLTF_COPYALPHA)
//			strf |= STRF_COPYALPHA;
//		//Stretch
//		return source->Stretch(*dest,dx,dy,rdPtr->bdW,rdPtr->bdH,sx,sy,sw,sh,
//			rdPtr->bM,rdPtr->bOp,rdPtr->bParam,strf);
//	}
//}