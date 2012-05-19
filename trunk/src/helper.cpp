/*
 *  zTranslate by rock88 (c)
 *  e-mail: rock88a@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include "helper.h"

extern int dot_array[1024];

static inline char toHexHelper(char c)
{
    static const char hexnumbers[] = "0123456789ABCDEF";
    return hexnumbers[c & 0xf];
}

static void q_toPercentEncoding(QCString *str, char percent)
{
    if (str->isEmpty())
        return;

    QCString input = *str;
    //int len = input.count();
	int len = str->length();
	printf("len=%i\n",len);
    const char *inputData = input.data();
    char *output = 0;
    int length = 0;

    for (int i = 0; i < len; ++i) {
        unsigned char c = *inputData++;
        if (((c >= 0x61 && c <= 0x7A) // ALPHA
             || (c >= 0x41 && c <= 0x5A) // ALPHA
             || (c >= 0x30 && c <= 0x39) // DIGIT
             || c == 0x2D // -
             || c == 0x2E // .
             || c == 0x5F // _
             || c == 0x7E // ~
             )) {
            if (output)
                output[length] = c;
            ++length;
        } else {
            if (!output) {
                // detach now
                str->resize(len*3); // worst case
                output = str->data();
            }
            output[length++] = percent;
            output[length++] = toHexHelper((c & 0xf0) >> 4);
            output[length++] = toHexHelper(c & 0xf);
        }
    }
    if (output)
        str->truncate(length);
}

static QCString toPercentEncodingHelper(const QString &s)
{
    if (s.isNull())
        return QCString();    // null
    QCString str = s.utf8();
    q_toPercentEncoding(&str, '%');
    return str;
}

QCString toPercentEncoding(const QString &input)
{
    return toPercentEncodingHelper(input);
}

QString decodeChar(QString str)
{
	int j = 0;
	
	memset(dot_array, 0x00, 1024);
	
	for(int i = 0; i < str.length();++i)
	{
		if (str.at(i) == QChar('.'))
		{
			str.at(i) = QChar(',');
			dot_array[j] = 0x01;
			j++;
			continue;
		}
		if (str.at(i) == QChar('?'))
		{
			str.at(i) = QChar(',');
			dot_array[j] = 0x02;
			j++;
			continue;
		}
		if (str.at(i) == QChar('!'))
		{
			str.at(i) = QChar(',');
			dot_array[j] = 0x03;
			j++;
			continue;
		}
		if (str.at(i) == QChar(','))
		{
			dot_array[j] = 0x04;
			j++;
			continue;
		}
	}
	
	return str;
}

QString encodeChar(QString str)
{
	int j = 0;
	for(int i = 0; i < str.length();++i)
	{
		//if (s1.at(i) == QChar('.')) dot_array[j] = 0x01;
		//printf("ok %c - %i[%i]\n",s1.at(i).latin1(),dot_array[j],j);
		if ((str.at(i) == QChar(',')) && (dot_array[j] == 0x01))
		{
			str.at(i) = QChar('.');
			j++;
		}
		if ((str.at(i) == QChar(',')) && (dot_array[j] == 0x02))
		{
			str.at(i) = QChar('?');
			j++;
		}
		if ((str.at(i) == QChar(',')) && (dot_array[j] == 0x03))
		{
			str.at(i) = QChar('!');
			j++;
		}
		if ((str.at(i) == QChar(',')) && (dot_array[j] == 0x04))
		{
			j++;
		}		
	}
	return str;
}