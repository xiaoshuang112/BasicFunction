YUV420SP_to_RGB24(HI_U8 *yuv420sp[2], HI_U8 *rgb24)
{

/*****************************************************************************/
    //	SaveBmpInfo(pfd, width, height);
    	//rgb24 = (HI_U8 *)malloc(Linebytes*height);
	//memset(rgb24, 0 , Linebytes*height);
    	int R,G,B;
	int x,y;
    	HI_U8 Y,U,V;	
    	for(y=0;y<height;y++)
    	{
        	for(x=0;x<width;x++)
  		   {
   	    		Y = *(yuv420sp[0] + y*width + x);
				U = *
   	    		V = *(yuv420sp[0] + ((y*width)>>1)+ 2*(x>>1) + 1);
	    				//the above code is get the Y U V data

			
   	    		R = Y + 1.402*(V-128);//method 1
   	    		G = Y - 0.34414*(U-128) - 0.71414*(V-128);
  	    		B = Y + 1.772*(U-128);
			
			/*
	  		R=1.164*(Y-16)+1.596*(V-128);//method 2
	  		G=1.164*(Y-16)+0.813*(V-128)-0.391*(U-128);
	  		B=1.164*(Y-16)+2.018*(U-128);
	  		*/
			/*
			R = Y + 1.4075*(V-128);//method 3
			G = Y - 0.3445*(U-128) - 0.7169*(V-128);
			B = Y + 1.779*(U-128);
			*/
			/*
			R = 1.164*(Y-16) + 1.596*(V-128);//method 4
			G = 1.164*(Y-16) - 0.392*(U-128) - 0.813*(V-128);
			B = 1.164*(Y-16) + 2.017*(U-128);			
			*/

	 		/*		correct the RGB data		*/ 
	   		if(R>255)	R=255;
	   		if(R<0)		R=0;
			
	   		if(G>255)	G=255;
	   		if(G<0)		G=0;
			
	   		if(B>255)	B=255;
	   		if(B<0)		B=0;
				
  			rgb24[((height-y-1)*width + x)*3 + 0] = (HI_U8)R;
	   		rgb24[((height-y-1)*width + x)*3 + 1] = (HI_U8)G;
	   		rgb24[((height-y-1)*width + x)*3 + 2] = (HI_U8)B;
  	    	}
        }
	//SaveBmpData(pfd, width, height, rgb24);
    	//fwrite(rgb24, Linebytes*height, 1, pfd);
    	//fclose(pfd);
    	//free(rgb24);

    	return rgb24;
}
