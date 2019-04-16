#include "stdio.h"
#include "string.h"


bool GetIniFileString(char *pFileName, char *pKeyName, char *pFieldName, char *pDest, unsigned int *pDestSize)
{
	bool bResult = false;
	
	FILE *pF=NULL;
	
	bool bFoundKey=false;
	bool bFoundField=false;
	
	char c=' ';
	char sKeyStr[100];
	char sFldStr[100];

	unsigned int i;
	

	// First Open the ini file
	pF = fopen(pFileName, "r");

	if(pF != NULL)
	{
		//First find the key
		do
		{
			char c=' ';
			while( !feof(pF) && c != '[')
			{
				c=fgetc(pF);
			}
			
			i = 0;
			
			do
			{
				c = fgetc(pF);
				sKeyStr[i] = c;
				i++;
			}
			while( !feof(pF) && (c != ']') );
			sKeyStr[i-1] = '\0';

			if( strncmp(sKeyStr, pKeyName, strlen(pKeyName)) == 0 )
				bFoundKey=true;
		}
		while (!feof(pF) && !bFoundKey);
		
		do
		{
			c=fgetc(pF);
		}
		while( !feof(pF) && (c != '\n') );
		
		// Next find the field
		if(bFoundKey)
		{
			do
			{
				i = 0;
				do
				{
					do
					{
						c=fgetc(pF);
					}
					while ((c=='\n') || (c=='\t') || (c==' ') );

					sFldStr[i] = c;
					i++;
				}
				while( !feof(pF) &&  (c != '=') );
				sFldStr[i-1] = '\0';

				if( strncmp(sFldStr, pFieldName, strlen(pFieldName)) == 0 )
					bFoundField=true;
				else
				{
					do
					{
						c=fgetc(pF);
					}
					while( !feof(pF) && (c != '\n') );
				}
				
			}
			while (!feof(pF) && !bFoundField);

			// At this point the field and the = sign are read
			// So read the value until the end of the line or until a ';'character

			i=0;
			do
			{
				c = fgetc(pF);
				pDest[i] = c;
				i++;
			}
			while( !feof(pF) &&  (c != '\n') && (c != ';') );
			pDest[i-1] = '\0';

			if(!feof(pF) )
			{
				*pDestSize = i-1;
				bResult = true;
			}


		}



		fclose(pF);
	}
	


	return bResult;
}