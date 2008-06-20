/* Extended string routines
 *
 * Copyright notice for this file:
 *  Copyright (C) 2004 Jason Oster (Parasyte)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/// \file
/// \brief various string manipulation utilities

#include "xstring.h"
#include <string>

///Upper case routine. Returns number of characters modified
int str_ucase(char *str) {
	unsigned int i=0,j=0; //mbg merge 7/17/06 changed to unsigned int

	while (i < strlen(str)) {
		if ((str[i] >= 'a') && (str[i] <= 'z')) {
			str[i] &= ~0x20;
			j++;
		}
		i++;
	}
	return j;
}


///Lower case routine. Returns number of characters modified
int str_lcase(char *str) {
	unsigned int i=0,j=0; //mbg merge 7/17/06 changed to unsigned int

	while (i < strlen(str)) {
		if ((str[i] >= 'A') && (str[i] <= 'Z')) {
			str[i] |= 0x20;
			j++;
		}
		i++;
	}
	return j;
}


///White space-trimming routine

///Removes whitespace from left side of string, depending on the flags set (See STRIP_x definitions in xstring.h)
///Returns number of characters removed
int str_ltrim(char *str, int flags) {
	unsigned int i=0; //mbg merge 7/17/06 changed to unsigned int

	while (str[0]) {
		if ((str[0] != ' ') || (str[0] != '\t') || (str[0] != '\r') || (str[0] != '\n')) break;

		if ((flags & STRIP_SP) && (str[0] == ' ')) {
			i++;
			strcpy(str,str+1);
		}
		if ((flags & STRIP_TAB) && (str[0] == '\t')) {
			i++;
			strcpy(str,str+1);
		}
		if ((flags & STRIP_CR) && (str[0] == '\r')) {
			i++;
			strcpy(str,str+1);
		}
		if ((flags & STRIP_LF) && (str[0] == '\n')) {
			i++;
			strcpy(str,str+1);
		}
	}
	return i;
}


///White space-trimming routine

///Removes whitespace from right side of string, depending on the flags set (See STRIP_x definitions in xstring.h)
///Returns number of characters removed
int str_rtrim(char *str, int flags) {
	unsigned int i=0; //mbg merge 7/17/06 changed to unsigned int

	while (strlen(str)) {
		if ((str[strlen(str)-1] != ' ') ||
			(str[strlen(str)-1] != '\t') ||
			(str[strlen(str)-1] != '\r') ||
			(str[strlen(str)-1] != '\n')) break;

		if ((flags & STRIP_SP) && (str[0] == ' ')) {
			i++;
			str[strlen(str)-1] = 0;
		}
		if ((flags & STRIP_TAB) && (str[0] == '\t')) {
			i++;
			str[strlen(str)-1] = 0;
		}
		if ((flags & STRIP_CR) && (str[0] == '\r')) {
			i++;
			str[strlen(str)-1] = 0;
		}
		if ((flags & STRIP_LF) && (str[0] == '\n')) {
			i++;
			str[strlen(str)-1] = 0;
		}
	}
	return i;
}


///White space-stripping routine

///Removes whitespace depending on the flags set (See STRIP_x definitions in xstring.h)
///Returns number of characters removed, or -1 on error
int str_strip(char *str, int flags) {
	unsigned int i=0,j=0; //mbg merge 7/17/06 changed to unsigned int
	char *astr,chr;

	if (!strlen(str)) return -1;
	if (!(flags & (STRIP_SP|STRIP_TAB|STRIP_CR|STRIP_LF))) return -1;
	if (!(astr = (char*)malloc(strlen(str)+1))) return -1;
	while (i < strlen(str)) {
		chr = str[i++];
		if ((flags & STRIP_SP) && (chr == ' ')) chr = 0;
		if ((flags & STRIP_TAB) && (chr == '\t')) chr = 0;
		if ((flags & STRIP_CR) && (chr == '\r')) chr = 0;
		if ((flags & STRIP_LF) && (chr == '\n')) chr = 0;

		if (chr) astr[j++] = chr;
	}
	astr[j] = 0;
	strcpy(str,astr);
	free(astr);
	return j;
}


///Character replacement routine

///Replaces all instances of 'search' with 'replace'
///Returns number of characters modified
int chr_replace(char *str, char search, char replace) {
	unsigned int i=0,j=0; //mbg merge 7/17/06 changed to unsigned int

	while (i < strlen(str)) {
		if (str[i] == search) {
			str[i] = replace;
			j++;
		}
		i++;
	}
	return j;
}


///Sub-String replacement routine

///Replaces all instances of 'search' with 'replace'
///Returns number of sub-strings modified, or -1 on error
int str_replace(char *str, char *search, char *replace) {
	unsigned int i=0,j=0; //mbg merge 7/17/06 changed to unsigned int
	int searchlen,replacelen;
	char *astr;

	searchlen = strlen(search);
	replacelen = strlen(replace);
	if ((!strlen(str)) || (!searchlen)) return -1; //note: allow *replace to have a length of zero!
	if (!(astr = (char*)malloc(strlen(str)+1))) return -1;
	while (i < strlen(str)) {
		if (!strncmp(str+i,search,searchlen)) {
			if (replacelen) memcpy(astr+j,replace,replacelen);
			i += searchlen;
			j += replacelen;
		}
		else astr[j++] = str[i++];
	}
	astr[j] = 0;
	strcpy(str,astr);
	free(astr);
	return j;
}

///Converts the provided data to a string in a standard, user-friendly, round-trippable format
std::string BytesToString(void* data, int len)
{
	char temp[16];
	if(len==1) {
		sprintf(temp,"%d",*(unsigned char*)data);
		return temp;
	} else if(len==2) {
		sprintf(temp,"%d",*(unsigned short*)data);
		return temp;
	} else if(len==4) {
		sprintf(temp,"%d",*(unsigned int*)data);
		return temp;		
	}
	std::string ret;
	ret.resize(len*2+2);
	char* str= (char*)ret.c_str();
	str[0] = '0';
	str[1] = 'x';
	str += 2;
	for(int i=0;i<len;i++)
	{
		int a = (((unsigned char*)data)[i]>>4);
		int b = (((unsigned char*)data)[i])&15;
		if(a>9) a += 'A'-10;
		else a += '0';
		if(b>9) b += 'A'-10;
		else b += '0';
		str[i*2] = a;
		str[i*2+1] = b;
	}
	return ret;
}

///returns -1 if this is not a hex string
int HexStringToBytesLength(std::string& str)
{
	if(str.size()>2 && str[0] == '0' && toupper(str[1]) == 'X')
		return str.size()/2-1;
	else return -1;
}

///parses a string in the same format as BytesToString
///returns true if success.
bool StringToBytes(std::string& str, void* data, int len)
{
	if(str.size()>2 && str[0] == '0' && toupper(str[1]) == 'X')
		goto hex;
	
	if(len==1) {
		int x = atoi(str.c_str());
		*(unsigned char*)data = x;
		return true;
	} else if(len==2) {
		int x = atoi(str.c_str());
		*(unsigned short*)data = x;
		return true;
	} else if(len==4) {
		int x = atoi(str.c_str());
		*(unsigned int*)data = x;
		return true;
	}
	//we can't handle it
	return false;
hex:
	int amt = len;
	int bytesAvailable = str.size()/2;
	if(bytesAvailable < amt)
		amt = bytesAvailable;
	const char* cstr = str.c_str()+2;
	for(int i=0;i<amt;i++) {
		char a = toupper(cstr[i*2]);
		char b = toupper(cstr[i*2+1]);
		if(a>='A') a=a-'A'+10;
		else a-='0';
		if(b>='A') b=b-'A'+10;
		else b-='0';
		unsigned char val = ((unsigned char)a<<4)|(unsigned char)b; 
		((unsigned char*)data)[i] = val;
	}

	return true;
}

#include <string>
#include <vector>
/// \brief convert input string into vector of string tokens
///
/// \note consecutive delimiters will be treated as single delimiter
/// \note delimiters are _not_ included in return data
///
/// \param input string to be parsed
/// \param delims list of delimiters.

std::vector<std::string> tokenize_str(const std::string & str,
                                      const std::string & delims=", \t")
{
  using namespace std;
  // Skip delims at beginning, find start of first token
  string::size_type lastPos = str.find_first_not_of(delims, 0);
  // Find next delimiter @ end of token
  string::size_type pos     = str.find_first_of(delims, lastPos);

  // output vector
  vector<string> tokens;

  while (string::npos != pos || string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delims.  Note the "not_of". this is beginning of token
      lastPos = str.find_first_not_of(delims, pos);
      // Find next delimiter at end of token.
      pos     = str.find_first_of(delims, lastPos);
    }

  return tokens;
}

//this code was taken from WINE (LGPL)
//http://google.com/codesearch?hl=en&q=+lang:c+splitpath+show:CPvw9Z-euls:_RSotQzmLeU:KGzljMEYFbY&sa=N&cd=9&ct=rc&cs_p=http://gentoo.osuosl.org/distfiles/Wine-20050524.tar.gz&cs_f=wine-20050524/programs/winefile/splitpath.c
void splitpath(const char* path, char* drv, char* dir, char* name, char* ext)
{
    const char* end; /* end of processed string */
	const char* p;	 /* search pointer */
	const char* s;	 /* copy pointer */

	/* extract drive name */
	if (path[0] && path[1]==':') {
		if (drv) {
			*drv++ = *path++;
			*drv++ = *path++;
			*drv = '\0';
		} else path+=2;
	} else if (drv)
		*drv = '\0';

	/* search for end of string or stream separator */
	for(end=path; *end && *end!=':'; )
		end++;

	/* search for begin of file extension */
	for(p=end; p>path && *--p!='\\' && *p!='/'; )
		if (*p == '.') {
			end = p;
			break;
		}

	if (ext)
		for(s=end; (*ext=*s++); )
			ext++;
	else
		for(s=end; *s++; )

	/* search for end of directory name */
	for(p=end; p>path; )
		if (*--p=='\\' || *p=='/') {
			p++;
			break;
		}

	if (name) {
		for(s=p; s<end; )
			*name++ = *s++;

		*name = '\0';
	} else
		for(s=p; s<end; )
			*s++;

	if (dir) {
		for(s=path; s<p; )
			*dir++ = *s++;

		*dir = '\0';
	}
}

//mbg 5/12/08
//for the curious, I tested U16ToHexStr and it was 10x faster than printf.
//so the author of these dedicated functions is not insane, and I will leave them.

static char TempArray[11];

uint16 FastStrToU16(char* s, bool& valid)
{
	int i;
	uint16 v=0;
	for(i=0; i < 4; i++)
	{
		if(s[i] == 0) return v;
		v<<=4;
		if(s[i] >= '0' && s[i] <= '9')
		{
			v+=s[i]-'0';
		}
		else if(s[i] >= 'a' && s[i] <= 'f')
		{
			v+=s[i]-'a'+10;
		}
		else if(s[i] >= 'A' && s[i] <= 'F')
		{
			v+=s[i]-'A'+10;
		}
		else
		{
			valid = false;
			return 0xFFFF;
		}
	}
	valid = true;
	return v;
}

char *U8ToDecStr(uint8 a)
{
	TempArray[0] = '0' + a/100;
	TempArray[1] = '0' + (a%100)/10;
	TempArray[2] = '0' + (a%10);
	TempArray[3] = 0;
	return TempArray;
}

char *U16ToDecStr(uint16 a)
{
	TempArray[0] = '0' + a/10000;
	TempArray[1] = '0' + (a%10000)/1000;
	TempArray[2] = '0' + (a%1000)/100;
	TempArray[3] = '0' + (a%100)/10;
	TempArray[4] = '0' + (a%10);
	TempArray[5] = 0;
	return TempArray;
}

char *U32ToDecStr(char* buf, uint32 a)
{
	buf[0] = '0' + a/1000000000;
	buf[1] = '0' + (a%1000000000)/100000000;
	buf[2] = '0' + (a%100000000)/10000000;
	buf[3] = '0' + (a%10000000)/1000000;
	buf[4] = '0' + (a%1000000)/100000;
	buf[5] = '0' + (a%100000)/10000;
	buf[6] = '0' + (a%10000)/1000;
	buf[7] = '0' + (a%1000)/100;
	buf[8] = '0' + (a%100)/10;
	buf[9] = '0' + (a%10);
	buf[10] = 0;
	return buf;
}
char *U32ToDecStr(uint32 a)
{
	return U32ToDecStr(TempArray,a);
}

char *U16ToHexStr(uint16 a)
{
	TempArray[0] = a/4096 > 9?'A'+a/4096-10:'0' + a/4096;
	TempArray[1] = (a%4096)/256 > 9?'A'+(a%4096)/256 - 10:'0' + (a%4096)/256;
	TempArray[2] = (a%256)/16 > 9?'A'+(a%256)/16 - 10:'0' + (a%256)/16;
	TempArray[3] = a%16 > 9?'A'+(a%16) - 10:'0' + (a%16);
	TempArray[4] = 0;
	return TempArray;
}

char *U8ToHexStr(uint8 a)
{
	TempArray[0] = a/16 > 9?'A'+a/16 - 10:'0' + a/16;
	TempArray[1] = a%16 > 9?'A'+(a%16) - 10:'0' + (a%16);
	TempArray[2] = 0;
	return TempArray;
}

std::string stditoa(int n)
{
	char tempbuf[16];
	sprintf(tempbuf, "%d", n);
	return tempbuf;
}

