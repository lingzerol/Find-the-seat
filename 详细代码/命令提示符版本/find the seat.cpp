

#include "stdafx.h"  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <io.h>  
#include "curl/curl.h"  
#include <ctime>
#include <iostream>

#define POSTURL "https://gupiao.baidu.com/stock/hk00700.html?from=aladingpc"  
#define FILEPATH "D:\\temp.txt"  


  


char *res_buf = NULL;
int shift;
int primer,p;
const int m = 123;
//int evident[60][100];//测试数据保存数组
const int position[20] = { 0,5,6,6,6,6,7,8,8,8,8,8,8,8,5,5,6,6,5,4 };
char canteen[60][100] = {
"                                           \n",
"                                           \n",
"                                           \n",
" G   O   W   G   B\n",
"                                           \n",
"                                           \n",
" B   G   G   R   G   R\n",
"                                           \n",
"                                           \n",
" R   B   G   Y   Y   O\n",
"                                           \n",
"                                           \n",
" B   B   G   W   G   W\n",
"                                           \n",
"                                           \n",
" G   B   R   G   B   O\n",
"                                           \n",
"                                           \n",
" O   W   B   B   W   B   B\n",
"                                           \n",
"                                           \n",
" G   W   G   W   O   O   R   W\n",
"                                           \n",
"                                           \n",
" O   W   B   R   B   R   O   O\n",
"                                           \n",
"                                           \n",
" B   O   W   G   W   B   R   R\n",
"                                           \n",
"                                           \n",
" G   R   G   B   B   B   G   G\n",
"                                           \n",
"                                           \n",
" G   G   G   R   W   G   W   W\n",
"                                           \n",
"                                           \n",
" B   R   W   W   O   W   G   G\n",
"                                           \n",
"                                           \n",
" R   O   B   B   B   B   B   G\n",
"                                           \n",
"                                           \n",
" B   B   G   R   W\n",
"                                           \n",
"                                           \n",
" G   W   W   B   W\n",
"                                           \n",
"                                           \n",
" B   G   W   O   W   O\n",
"                                           \n",
"                                           \n",
" W   W   O   B   G   R\n",
"                                           \n",
"                                           \n",
" G   B   W   O   G\n",
"                                           \n",
"                                           \n",
" B   R   O   B\n"
"                                           \n",
"                                           \n",
};


//这是libcurl接收数据的回调函数，相当于recv的死循环  
//其中stream可以自定义数据类型，这里我传入的是文件保存路径  
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	int res_size;

	res_size = size * nmemb;
	res_buf = (char *)realloc(res_buf, shift + res_size + 1);
	memcpy(res_buf + shift, ptr, res_size);
	shift += res_size;
	return size * nmemb;
}


int GetUrl(const char *url, char *savepath)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *chunk = NULL;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		//指定回调函数  
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		//这个变量可作为接收或传递数据的作用  
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, savepath);
		res = curl_easy_perform(curl);
		if (res == CURLE_OK)
		{
			return 1;
		}
		return 0;
	}

	return 0;
}
bool check(int i) {
	return (res_buf[i] >= '0'&&res_buf[i] <= '9') && (res_buf[i + 1] >= '0'&&res_buf[i + 1] <= '9') && (res_buf[i + 2] >= '0'&&res_buf[i + 2] <= '9') && (res_buf[i + 3] == '.') && (res_buf[i + 4] >= '0'&&res_buf[i + 4] <= '9') && (res_buf[i + 5] >= '0'&&res_buf[i + 5] <= '9');
}

bool get_stock(char *stock) {
	int i, j;
	for (i = 0; i < strlen(res_buf); ++i) {
		if (check(i)) {
			for (j = 0; j < 6; ++j)
				stock[j] = res_buf[i + j];
			stock[6] = '\0';
			break;
		}
	}
	if (i == strlen(res_buf)) {
		return false;
	}
	else return true;
}

void get_data(tm *&data) {
	time_t now=time(NULL);
	data = localtime(&now);
}
int find_primer(tm *data,double stock) {
	int s = (int)((stock - (int)stock) * 100);
	s = s == 0 ? stock : s;
	for (int i = data->tm_mday*s/10;; ++i) {
		int j;
		for (j = 2; j < i / 2; ++j) {
			if (i%j == 0) {
				break;
			}
		}
		if (j == i / 2) {
			primer = i;
			break;
		}
	}
	for (int i = data->tm_mday*data->tm_year*s; i >= 2; --i) {
		int j;
		for (j = 2; j < i / 2; ++j) {
			if (i%j == 0) {
				break;
			}
		}
		if (j == i / 2) {
			p = i;
			break;
		}
	}
	return 0;
}
int get_a(tm *data,double s) {
	int a;
	int sum;
	sum = data->tm_mday + data->tm_mon + data->tm_wday + data->tm_yday + data->tm_year+ (int)((s - (int)s) * 100);
	sum %= primer;
	a = sum*sum;
	return a;
}

int get_b(tm *data,double s) {
	int b;
	int sum;
	sum = data->tm_mday * data->tm_mon * data->tm_wday * data->tm_yday * data->tm_year*(int)((s-(int)s)*100);
	sum %= primer;
	b = sum*sum;
	return b;
}

int Hash(int x,tm *data,int a,int b) {
	return (a*x + b) % p;
}

int seat(int hash1, int hash2, int hash3, int hash4, int hash5) {
	return (primer*(primer*(primer*(primer*hash1 + hash2)%m + hash3)%m + hash4)%m + hash5) % m;
}

void output(double stock, tm *data, int s) {
	int i, j;
	printf("Today is: %d-%d-%d\n", data->tm_year + 1900, data->tm_mon + 1, data->tm_mday);
	printf("The price of th tencent stock is: %.2lf\n", stock);
	i = 1;
	while (s > position[i]) {
		s -= position[i++];
	}

	j = s;
	printf("Next time we meet at: %d row, %d column\n", i, j);
	i *= 3;
	canteen[i - 1][j + (j - 1) * 3] = '-';
	canteen[i][j + (j - 1) * 3 - 1] = '|';
	canteen[i + 1][j + (j - 1) * 3] = '-';
	canteen[i][j + (j - 1) * 3 + 1] = '|';
	for (int k = 0; k < 60; ++k) {
		printf("%s", canteen[k]);
	}
	canteen[i - 1][j + (j - 1) * 3] = ' ';
	canteen[i][j + (j - 1) * 3 - 1] = ' ';
	canteen[i + 1][j + (j - 1) * 3] = ' ';
	canteen[i][j + (j - 1) * 3 + 1] = ' ';

}


void test(double stock, tm *data) {
	int year, month, day, hash1, hash2, hash3, hash4, hash5, s;
	int c, r;
	int i, j;
	int a, b;
	year = data->tm_year + 1900;
	month = data->tm_mon + 1;
	day = data->tm_mday;
	find_primer(data,stock);
	a = get_a(data,stock);
	b = get_b(data,stock);
	hash1 = Hash(year, data,a,b);
	hash2 = Hash(month, data,a,b);
	hash3 = Hash(day, data,a,b);
	hash4 = Hash((int)stock, data,a,b);
	hash5 = Hash((stock - (int)stock) * 100, data,a,b);
	s = seat(hash1, hash2, hash3, hash4, hash5)+1;

	i = 1;
	while (s > position[i]) {
		s -= position[i++];
	}
	j = s;
	evident[i][j]++;
}

void find_the_seat(double stock,tm *data) {
	int year, month, day,hash1,hash2,hash3,hash4,hash5,s;
	int c, r;
	int i, j;
	int a, b;
	year = data->tm_year + 1900;
	month = data->tm_mon + 1;
	day = data->tm_mday;
	find_primer(data,stock);
	a = get_a(data, stock);
	b = get_b(data, stock);
	hash1 = Hash(year, data, a, b);
	hash2 = Hash(month, data, a, b);
	hash3 = Hash(day, data, a, b);
	hash4 = Hash((int)stock, data, a, b);
	hash5 = Hash((stock - (int)stock) * 100, data, a, b);
	s = seat(hash1, hash2, hash3, hash4, hash5);
	output(stock, data, s+1);
}

int main()
{	

	char stock[10];
	double s;
	tm *data;
	if (!GetUrl(POSTURL, FILEPATH))
	{
		printf("cannot connect to the Internet.......\n");
		return -1;
	}
	if (!get_stock(stock)) {
		printf("can not get the stock......\n");
		return -1;
	}
	s = atof(stock);
	get_data(data);
	find_the_seat(s,data);
	/*data = new tm;
	//for(int k=100;k<150;++k)
	for (int i = 1; i <= 12; ++i) {
		for (int j = 1; j <= 31; ++j) {
			//for (int o = 1; o <= 7; ++o)
			{//for (int l = 1; l <= 365; ++l) {
				data->tm_year = 117;
				data->tm_mday = j;
				data->tm_mon = i;
				data->tm_wday = 5;
				data->tm_yday = 150;
				test(s, data);
	
				//}
			}
		}
	}
	for (int i = 0; i < 60; ++i) {
		for (int j = 0; j < 100; ++j) {
			printf("%d  ", evident[i][j]);
		}
		printf("\n");//用于测试
	}*/
	system("pause");
	return 0;
}