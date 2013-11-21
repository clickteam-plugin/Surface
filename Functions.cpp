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

COLORREF CALLBACK OpBlitProc(COLORREF dest, COLORREF src, DWORD lParam)
{
	LPRDATA rdPtr = (LPRDATA)lParam;

	BYTE* srcBytes = (BYTE*)&src;
	BYTE* destBytes = (BYTE*)&dest;

	TCHAR* op = rdPtr->b.procOp;

	float cr = srcBytes[0]*rdPtr->b.procOpSrc;
	float cg = srcBytes[1]*rdPtr->b.procOpSrc;
	float cb = srcBytes[2]*rdPtr->b.procOpSrc;

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
	if(rdPtr->b.srcUse && (rdPtr->b.srcW <= 0 || rdPtr->b.srcH <= 0))
		return false;
	if(rdPtr->b.StretchMode && (rdPtr->b.destW <= 0 || rdPtr->b.destH <= 0))
		return false;

	//Some important variables
	int dx = rdPtr->b.destX;
	int dy = rdPtr->b.destY;
	int sx = rdPtr->b.srcUse ? rdPtr->b.srcX : 0;
	int sy = rdPtr->b.srcUse ? rdPtr->b.srcY : 0;
	int sw = source->GetWidth();
	int sh = source->GetHeight();

	//Prepare hot spot
	int hotX = rdPtr->b.hotX;
	int hotY = rdPtr->b.hotY;
	if(rdPtr->b.hotspotMode & 2)
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
	if(rdPtr->b.StretchMode==2) strf |= STRF_RESAMPLE;
	else if(rdPtr->b.StretchMode==3) strf |= STRF_RESAMPLE_TRANSP;

#ifdef HWABETA
	/* HWA */
	if (rdPtr->isHWA && source->GetType() >= ST_HWA_RTTEXTURE && dest->GetType() >= ST_HWA_SCREEN)
	{
		// Use region if necessary
		if(rdPtr->b.srcUse)
		{
			sx = max(0,min(sw,sx));
			sy = max(0,min(sh,sy));
			sw = max(0,min(rdPtr->b.srcW,sw));
			sh = max(0,min(rdPtr->b.srcH,sh));
		}

		if(!sw || !sh)
			return false;

		POINT hotspot = {0, 0};

		if(rdPtr->b.hotspotMode)
		{
			hotspot.x = hotX;
			hotspot.y = hotY;
		}
		
		float scaleX = 1;
		float scaleY = 1;

		if(rdPtr->b.StretchMode)
		{
			scaleX = (rdPtr->b.destW * 1.0f) / sw;
			scaleY = (rdPtr->b.destH * 1.0f) / sh;
		}

		float angle = rdPtr->b.angle;

		// BOP_BLEND expects 0-128 but we can use 0-255 all the time
		DWORD bop = rdPtr->b.operation;
		if (bop == BOP_BLEND)
			bop = BOP_COPY;
		return source->BlitEx(*dest, dx, dy, scaleX, scaleY, sx, sy, sw, sh, &hotspot, angle, rdPtr->b.mode, (BlitOp)(bop | BOP_RGBAFILTER), rdPtr->b.param, strf);
	}
#endif

	//Compose alpha
	if(rdPtr->b.composeAlpha && rdPtr->b.mode == BMODE_TRANSP)
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
		int x2 = rdPtr->b.srcUse ? (x1+rdPtr->b.srcW) : sw;
		int y2 = rdPtr->b.srcUse ? (y1+rdPtr->b.srcH) : sh;

		x1 = max(0,min(sw,x1));
		y1 = max(0,min(sh,y1));
		x2 = max(0,min(sw,x2));
		y2 = max(0,min(sh,y2));

		//Nothing to do
		if(x1 >= x2 || y1 >= y2)
			return false;
		
		//Scale if necessary
		cSurface scaled;
		if(rdPtr->b.StretchMode)
		{
			scaled.Create(rdPtr->b.destW,rdPtr->b.destH,source);
			scaled.SetTransparentColor(source->GetTransparentColor());
			source->Stretch(scaled,0,0,rdPtr->b.destW,rdPtr->b.destH,x1,y1,x2-x1,y2-y1,BMODE_OPAQUE,BOP_COPY,0,STRF_COPYALPHA|(rdPtr->b.StretchMode-1));
			source = &scaled;
			dx += x1;
			dy += y1;
			hotX *= rdPtr->b.destW*1.0f/sw; //(x2-x1) makes hotspot relative to region
			hotY *= rdPtr->b.destH*1.0f/sh; //(y2-y1)
			sw = rdPtr->b.destW;
			sh = rdPtr->b.destH;
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
		if(rdPtr->b.angle != 0)
		{
			double angle = rdPtr->b.angle/180.0*PI;
			cSurface region;
			region.Create(x2-x1,y2-y1,source);
			source->Blit(region,-x1,-y1,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
			region.CreateRotatedSurface(rotated,rdPtr->b.angle,rdPtr->b.angleResample,source->GetTransparentColor(),FALSE);
			source = &rotated;
			//Rotate the hotspot and adjust the rotated bounding box
			dx += x1;
			dy += y1;
			if(rdPtr->b.hotspotMode & 1)
				RotatePoint(angle,&hotX,&hotY,region.GetWidth(),region.GetHeight(),&dx,&dy);
			sw = rotated.GetWidth();
			sh = rotated.GetHeight();
			x1 = 0;
			y1 = 0;
			x2 = sw;
			y2 = sh;
		}

		//Apply hot spot
		if(rdPtr->b.hotspotMode & 1)
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
		if(rdPtr->b.callback)
			rdPtr->callback = rdPtr->b.callback;
		//Use semi-transparency
		else if(rdPtr->b.operation == BOP_BLEND)
			factor *= ((rdPtr->b.param & 0xff000000) >> 24)/255.0f;

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
				if(!rdPtr->b.callback)
				{
					
					switch(rdPtr->b.operation)
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
	if(rdPtr->b.StretchMode)
	{
		dw = rdPtr->b.destW;
		dh = rdPtr->b.destH;
	}
	if(dw <= 0 || dh <= 0)
		return false;

	//Recursive call for rotated images
	if(rdPtr->b.angle != 0)
	{
		//Create a fake rdPtr with destination (0,0) and angle 0
		RUNDATA fakePtr;
		memcpy((void*)&fakePtr,rdPtr,sizeof(RUNDATA));
		fakePtr.b.destX = 0;
		fakePtr.b.destY = 0;
		fakePtr.b.angle = 0;
		fakePtr.b.callback[0] = 0;
		fakePtr.b.operation = BOP_COPY;
		fakePtr.b.hotspotMode = 0;
		//Blit our source image onto a temporary image
		cSurface rotsource;
		rotsource.Create(dw,dh,source);
		if(Blit(source,&rotsource,&fakePtr))
		{
			//Rotate the image and blit onto the destination image
			cSurface rotated;
			rotsource.CreateRotatedSurface(rotated,rdPtr->b.angle,rdPtr->b.angleResample,source->GetTransparentColor(),FALSE);
			fakePtr.b.destX = rdPtr->b.destX;
			fakePtr.b.destY = rdPtr->b.destY;
			memcpy(fakePtr.b.callback, rdPtr->b.callback, sizeof(rdPtr->b.callback));
			fakePtr.b.operation = rdPtr->b.operation;
			fakePtr.b.StretchMode = 0;
			fakePtr.b.srcUse = 0;
			fakePtr.b.hotspotMode = 0;
			if(rdPtr->b.hotspotMode & 1)
			{
				int dw = sw;
				int dh = sh;
				if(rdPtr->b.StretchMode)
				{
					dw = rdPtr->b.destW;
					dh = rdPtr->b.destH;
					hotX *= dw*1.0f/sw;
					hotY *= dh*1.0f/sh;
				}
				if(rdPtr->b.angle != 0)
					RotatePoint(rdPtr->b.angle/180.0*PI,&hotX,&hotY,dw,dh,&fakePtr.b.destX,&fakePtr.b.destY);
				fakePtr.b.destX -= hotX;
				fakePtr.b.destY -= hotY;
			}
			return Blit(&rotated,dest,&fakePtr);
		}
		return false;
	}

	//No rotation, but apply hotspot
	if(rdPtr->b.hotspotMode & 1)
	{
		dx -= hotX*dw*1.0f/sw;
		dy -= hotY*dh*1.0f/sh;
	}

	//Use region if necessary
	if(rdPtr->b.srcUse)
	{
		sx = max(0,min(sw,sx));
		sy = max(0,min(sh,sy));
		sw = max(0,min(rdPtr->b.srcW,sw));
		sh = max(0,min(rdPtr->b.srcH,sh));
	}
	//Just to make sure...
	else
	{
		sx = 0;
		sy = 0;
		sw = source->GetWidth();
		sh = source->GetHeight();
	}

	// Classic 0-128 semi transparency
	LPARAM semiTransparency = 128 - ((rdPtr->b.param & 0xff000000) >> 24)*128.0f/255;

	//Blitting
	if(!rdPtr->b.StretchMode)
	{
		//Filter callback blit
		if(rdPtr->b.callback[0])
		{
			rdPtr->callback = rdPtr->b.callback;
			return source->FilterBlit(*dest,dx,dy,sx,sy,sw,sh,rdPtr->b.mode,CallBlitProc,(LPARAM)rdPtr);
		}

		//Operation blit
		if(rdPtr->b.procOp[0])
			return source->FilterBlit(*dest,dx,dy,sx,sy,sw,sh,rdPtr->b.mode,OpBlitProc,(LPARAM)rdPtr);

		//Simple blit
		return source->Blit(*dest,dx,dy,sx,sy,sw,sh,
			rdPtr->b.mode,rdPtr->b.operation,semiTransparency,rdPtr->b.flags);
	}
	//Stretching
	else
	{
		//Copy alpha
		if(rdPtr->b.flags & BLTF_COPYALPHA)
			strf |= STRF_COPYALPHA;
		//Stretch
		return source->Stretch(*dest,dx,dy,rdPtr->b.destW,rdPtr->b.destH,sx,sy,sw,sh,
			rdPtr->b.mode,rdPtr->b.operation,semiTransparency,strf);
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
#ifdef HWABETA
		cSurface::GetSizeOfRotatedRect(&width, &height, *(float*)&rdPtr->rc.rcAngle); // What
#else
		printf("UH: Scaled: %d,%d\n", width, height);
		cSurface::GetSizeOfRotatedRect(&width, &height, rdPtr->rc.rcAngle);
		printf("UH: Rotated: %d,%d\n", width, height);
#endif
	}

	//Update size and scale image
	rdPtr->rHo.hoImgWidth = width; // leave room for rounding errors
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

void Operation(TCHAR* op,BYTE* a,float b)
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