
//自动排版	Alt+F8

#include <stdio.h>
#include <windows.h>//system
#include <iostream>
#include <conio.h>//puts
#include <string> 
#include <io.h>
using namespace std;
#define fn 100
#define fl 255
#define fenjien 10
#define fenjiel 100
#define Dizhi "E:\\Temporary\\1Check\\2Check\\(Batch)"
#define debugMode 0//0:关。1:开。

int duquwenjianming(char save[fn][fl])
{
    system("dir /b "Dizhi">>buf.txt");//将目录导出到buf.txt
	
    FILE *fp = fopen("buf.txt","rb");
	
	int i=0;
    while (!feof(fp))
    {		
        fgets(save[i],fl,fp);
		i++;
    }
	
    fclose(fp);
	
	system("del /Q buf.txt");  //删除目录下buf.txt文件
	
	return i-1;
}


int radetext(char text[fn][fenjiel],char * path)
{
	
    FILE *fp = fopen(path,"rb");
	
	int i=0;
    while (!feof(fp))
    {		
        fgets(text[i],fenjiel,fp);
		//	cout<<text[i]<<endl;
		i++;
    }
	
    fclose(fp);
	
	return i-1;
}

void viewshuzu(char save[fn][fl],int strn)//查看字符数组
{
	for(int i=0;i<strn;i++)
	{
		//		cout<<"*";
		puts(save[i]); //显示文件名
		//		cout<<"*";
	}
	//cout<<endl;
}

void viewshuzu(char save[fn][fenjiel],int strn)//查看字符数组
{
	for(int i=0;i<strn;i++)
	{
		//		cout<<"*";
		puts(save[i]); //显示文件名
		//		cout<<"*";
	}
	//cout<<endl;
}

void viewshuzu1(int intshuzu[fenjien])//查看整型数组
{
	for(int i=0;i<fenjien;i++)
	{
		cout<<intshuzu[i]<<" ";
	}
	cout<<endl;
}

void viewshuzu1(int intshuzu[fn],int rfn)//查看整型数组
{
	for(int i=0;i<rfn;i++)
	{
		cout<<intshuzu[i]<<" ";
	}
	cout<<endl;
}

int viewshuzu2(string stringshuzu[fenjien])//查看String数组
{
	for (int i=0;i<10;i++)
	{
		cout<<i<<"*"<<stringshuzu[i]<<"*";
		cout<<endl;
	}
	cout<<endl;
	return i;
}

void myfind(int chu[fenjien],char *zhao,string zifu)//查找字符中含《》的所有位置
{
	int i=1;
	if (zifu.find(zhao)<zifu.size())
	{
		chu[0]=zifu.find(zhao);
		while(zifu.find(zhao,chu[i-1]+1)<zifu.size())
		{
			chu[i]=zifu.find(zhao,chu[i-1]+1);
			i++;
		}
	}
}

int str2char(string str,char cha[fl])
{
	int suc=0;
	int i=0;
	//	cout<<str.length()<<endl;
	if(str.length()!=0)//数组不为空
	{
		for(;i<str.length();i++)
			cha[i] = str[i];
		//		cha[i] = '\n';//每行包括一个换行符 终止符
		//		cout<<cha<<endl;//浏览转换结果
		suc=1;
		cha[i] = '\0';
	}
	else
	{
		//		cha[0] = '\n';
		suc=2;
	}
	
	while (cha[i]==13 || cha[i]==10 || cha[i]==32 || cha[i]==0)
	{
		cha[i]='\0';
		i--;
	}
	if (debugMode)
	{
		cout<<"str2char:"<<endl<<cha<<endl<<endl;
	}
	
	return suc;
}

int str2char2(string str,char cha[fenjiel])
{
	int suc=0;
	int i=0;
	//	cout<<str.length()<<endl;
	if(str.length()!=0)//数组不为空
	{
		for(;i<str.length();i++)
			cha[i] = str[i];
		//		cha[i] = '\n';//每行包括一个换行符 终止符
		//		cout<<cha<<endl;//浏览转换结果
		suc=1;
		cha[i] = '\0';
	}
	else
	{
		//		cha[0] = '\n';
		suc=2;
	}
	
	while (cha[i]==13 || cha[i]==10 || cha[i]==32 || cha[i]==0)
	{
		cha[i]='\0';
		i--;
	}
	//cout<<"str2char2:"<<cha<<endl;
	
	return suc;
}

int eareSearch(int lowEdge,int upEdge,int eare[fenjien])//查找一个范围内是否包含一个数组中的数字
{
	for (int i=0;i<fenjien;i++)
	{
		if (lowEdge<eare[i] && eare[i]<upEdge)
			return 1;
	}
	return 0;
}

string fenjie(char save1[fl],string changeInfoStr[fn],int strn,int* wrongCode)//分解字符数组到可分析形式
{
	string save11=save1;
	string save111[10];
	string tempstr;
	string empty;
	string jieguo;
	
	if (*wrongCode!=0)//有错误直接跳出
	{
		jieguo.append(save11);
		return jieguo;
	}
	
	int ndq[fenjien]={0};
	int ndh[fenjien]={0};
	int nxq[fenjien]={0};
	int nxh[fenjien]={0};
	int tempint[fenjien]={0};
	int temp=0;
	int unknown=0;
	int unknownMark=0;
	
	//预处理
	int idx=0;
	//cout<<save11<<endl;
	
	while (1)//变【】为[]
	{
		idx=save11.find("【");
		if (idx==-1)
			break;
		save11.replace(idx,2,"[");
	}
	
	while (1)//变【】为[]
	{
		idx=save11.find("】");
		if (idx==-1)
			break;
		save11.replace(idx,2,"]");
	}
	
	while (1)//变（）为()
	{
		idx=save11.find("（");
		if (idx==-1)
			break;
		save11.replace(idx,2,"(");
	}
	
	while (1)//变（）为()
	{
		idx=save11.find("）");
		if (idx==-1)
			break;
		save11.replace(idx,2,")");
	}
	
	while (1)//～变~
	{
		idx=save11.find("～");
		if (idx==-1)
			break;
		save11.replace(idx,2,"~");
	}
	//	cout<<save11<<endl;
	
	idx=save11.find("[DL版]");//消除[DL版]
	if (idx!=-1)
		save11.replace(idx,6,"");
	
	idx=save11.find("[無修正]");//消除[無修正]
	if (idx!=-1)
		save11.replace(idx,8,"");
	
	idx=save11.find("(オリジナル)");//消除(オリジナル)
	if (idx!=-1)
		save11.replace(idx,12,"");
	
	idx=save11.find("(よろず)");//消除(よろず)
	if (idx!=-1)
		save11.replace(idx,8,"");
	
	idx=save11.find("[Chinese]");//未知汉化组
	if (idx!=-1)
	{
		unknown=1;
		save11.replace(idx,9,"");
	}
	
	idx=save11.find("[中国翻訳]");//未知汉化组
	if (idx!=-1)
	{
		unknown=1;
		save11.replace(idx,10,"");
	}
	
	idx=save11.find("★");//未知汉化组
	if (idx!=-1)
	{
		unknownMark=1;
	}

	myfind(ndq,"[",save11);//查找[所在位置
	myfind(ndh,"]",save11);//查找]所在位置
	myfind(nxq,"(",save11);//查找(所在位置
	myfind(nxh,")",save11);//查找)所在位置
	
	//viewshuzu1(ndq);
	//viewshuzu1(ndh);
	//viewshuzu1(nxq);
	//viewshuzu1(nxh);	
	
	for (int i=1;i<fenjien;i++)
	{
		if ((ndq[i]*ndh[i]==0 && ndq[i]+ndh[i]!=0) || (nxq[i]*nxh[i]==0 && nxq[i]+nxh[i]!=0))//序列中只有一个为0
		{
			*wrongCode=2;//Brackets inspect might be wrong
			jieguo.append(save11);
			return jieguo;
		}
	}
	
	if (ndh[0]!=0 || nxh[0]!=0)//包含[]或()的文件名
	{
		int pretreatment=0;//作者栏不包含()
		if (ndh[1]==0 && !eareSearch(ndq[0],ndh[0],nxh))//只有一个[]且不包含()
		{
			save11.insert(ndh[0],"()");//添加形式()
			pretreatment=1;
		}
		else
		{
			if (ndh[1]!=0 && !eareSearch(ndq[0],ndh[0],nxh) && !eareSearch(ndq[1],ndh[1],nxh))//有2个[]且不包含()
			{
				if (ndh[1]>save11.length()-4)//"[]"在最后，即汉化组在后
				{
					save11.insert(ndh[0],"()");//添加形式()
					pretreatment=1;
				}
				else
				{
					save11.insert(ndh[1],"()");//添加形式()
					pretreatment=1;
				}
			}
		}
		
		if (pretreatment==1)
		{
			myfind(ndq,"[",save11);//查找[所在位置
			myfind(ndh,"]",save11);//查找]所在位置
			myfind(nxq,"(",save11);//查找(所在位置
			myfind(nxh,")",save11);//查找)所在位置
		}
		
		//cout<<save11<<endl;
		
		pretreatment=0;//文件名没有以)或]结尾
		if (save11.find(')',save11.length()-4)==-1 && save11.find(']',save11.length()-4)==-1)//文件名没有以)或]结尾
		{
			save11.insert(save11.length(),"()");//添加形式()
			pretreatment=1;
		}
		
		if (pretreatment==1)
		{
			myfind(nxq,"(",save11);//查找(所在位置
			myfind(nxh,")",save11);//查找)所在位置
		}
		
		//cout<<save11<<endl;
		
		pretreatment=0;//汉化组在最后时，没有原作名
		if (save11.rfind(']')>save11.length()-4)//汉化组在最后
		{
			if (save11.rfind(')')!=save11.rfind('[')-1 && save11.rfind(')')!=save11.rfind('[')-2)//汉化组前没有原作名
			{
				save11.insert(save11.rfind('['),"()");
				pretreatment=1;
			}
		}
		
		if (pretreatment==1)
		{
			myfind(ndq,"[",save11);//查找[所在位置
			myfind(ndh,"]",save11);//查找]所在位置
			myfind(nxq,"(",save11);//查找(所在位置
			myfind(nxh,")",save11);//查找)所在位置
		}
	}
	else//没有[]或()的文件名直接输出
	{
		jieguo.append(save11);
		return jieguo;
	}
	
	if (debugMode)
	{
		cout<<"Brackets sequence detection:"<<endl;
		cout<<"ndq:";
		viewshuzu1(ndq);
		cout<<"ndh:";
		viewshuzu1(ndh);
		cout<<"nxq:";
		viewshuzu1(nxq);
		cout<<"nxh:";
		viewshuzu1(nxh);
		
		cout<<"Length:"<<save11.length()<<endl<<endl;
		
		cout<<"Pretreatment results:"<<endl<<save11<<endl<<endl;//预处理结果
	}
	
	////////////////////////////////////////
	//////////      预处理完      //////////
	////////////////////////////////////////
	
	
	//////////未知汉化组标记
	if(unknown==1)//未知汉化组
	{
		save111[0].append("★");
		//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
	}
	
	
	//////////提取[  ( )]作者所在内容
	int authorLoc=0;
	for(i=0;ndh[i]!=0;i++)
	{
		tempstr.append(save11,ndq[i],ndh[i]-ndq[i]+1);
		temp=tempstr.find("(");//检验是否有"("
		if(temp!=-1)
		{
			if(tempstr.rfind(" ",temp)!=temp-1)
			{
				tempstr.insert(temp," ");
			}
			
			save111[2].append(tempstr);
			//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
			
			tempstr=empty;
			authorLoc=i;
			break;
		}
		tempstr=empty;
	}
	
	if (debugMode)
	{		   
		cout<<"Extract author name result:"<<endl;
		viewshuzu2(save111);//提取作者结果
	}
	
	
	//////////提取")] ("中字符，即作品名
	int ndhfix=0,nxhfix=0;
	for(i=0;i<fenjien;i++)//寻找")]"的位置
	{
		for(int j=0;j<fenjien;j++)
		{	
			if(nxh[i]==ndh[j]-1)//满足")]"的位置
			{
				ndhfix=j;
				nxhfix=i;
				goto OT0;
			}
		}
	}
OT0:;
	int qx=0,hx=0,originWorkNameLoc=0;
	for(i=nxhfix+1;i<fenjien;i++)//在满足")]"的位置后搜寻满足")["条件或以")"结尾的位置
	{
		for(int j=ndhfix+1;j<fenjien;j++)
		{		
			if (nxh[i]==ndq[j]-1 || nxh[i]==ndq[j]-2 || nxh[i]>save11.length()-4)//满足")["或") ["条件或以")"结尾
			{
				originWorkNameLoc=i;
				
				tempstr.append(save11,ndh[ndhfix]+1,nxq[i]-ndh[ndhfix]-1);
				
				while(temp=tempstr.find(" ")==0)//检验首位是否有" "
				{
					tempstr=empty;
					qx++;
					tempstr.append(save11,ndh[ndhfix]+1+qx,nxq[i]-ndh[ndhfix]-1-qx);
				}
				
				
				while(temp=tempstr.rfind(" ")==tempstr.size()-1)//检验末位是否有" "
				{
					tempstr=empty;
					hx++;
					tempstr.append(save11,ndh[ndhfix]+1+qx,nxq[i]-ndh[ndhfix]-1-qx-hx);
				}
				
				//cout<<"*"<<tempstr<<"*"<<endl;
				
				save111[4].append(tempstr);
				//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
				
				tempstr=empty;
				goto OT1;
				
			}			
		}
	}
OT1:;
	
	if (debugMode)
	{		   
		cout<<"Extract book name results:"<<endl;
		viewshuzu2(save111);//提取本子名结果
	}
	

	//////////提取原作名
	int changed=0;//转化标记
	for(i=0;i<(strn+1)*2;i++,i++)
	{
		if(save11.rfind(changeInfoStr[i])!=-1)//发现要转化的作品
		{
			save111[6].append(changeInfoStr[i+1]);
			//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
			
			changed=1;
			break;
		}
	}
	if (changed==0)//没有需要转化的
	{
		save111[6].append(save11,nxq[originWorkNameLoc],nxh[originWorkNameLoc]-nxq[originWorkNameLoc]+1);
		//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
	}
	
	if (debugMode)
	{		   
		cout<<"Extract original name result:"<<endl;
		viewshuzu2(save111);//提取原作名结果
	}
	

	//////////提取汉化信息
	if(authorLoc==1)//汉化组信息在首位
	{
		save111[8].append(save11,ndq[0],ndh[0]-ndq[0]+1);//移到末尾
		//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
	}
	else//汉化组信息不在首位
	{
		if (ndh[1]!=0)//有超过2个大括号
		{
			for (i=1;i<fenjien;i++)
			{
				if(ndh[i+1]==0)//汉化组信息在末位
				{
					save111[8].append(save11,ndq[i],ndh[i]-ndq[i]+1);//移到末尾
					//0-1:标记 2-3:作者 4-5:作品名 6-7:原作 8-9:汉化组
					break;
				}
			}
		}
	}
	
	if (save111[8].empty() && unknownMark==1)
	{
		save111[0].append("★");//未补全汉化信息
	}

	if (debugMode)
	{
		cout<<"Decomposition results:"<<endl;
		viewshuzu2(save111);//所有结果
	}
	

	//////////拼接所有信息
	for(i=0;i<10;i++)
	{
		if (i==0)
		{
			idx=save111[i].find("()");//去掉形式()
			if (idx!=-1)
			{
				if (save111[i].length()==2)//"()"单独为一项
					save111[i].erase(idx,2);
				else//"()"在其他项中
					save111[i].erase(idx-1,3);
			}
		}
		idx=save111[i+1].find("()");//去掉形式()
		if (idx!=-1)
		{
			if (save111[i+1].length()==2)//"()"单独为一项
				save111[i+1].erase(idx,2);
			else//"()"在其他项中
				save111[i+1].erase(idx-1,3);
		}
		
		jieguo.append(save111[i]);
		//cout<<!save111[i].empty();
		if(!save111[i+1].empty() && !jieguo.empty() && i!=9)
		{
			jieguo.append(" ");
		}
		//cout<<"*"<<jieguo<<"*"<<endl;
	}
	
	if (debugMode)
	{
		cout<<"Process result:"<<endl;
		cout<<"*"<<jieguo<<"*"<<endl<<endl;
		cout<<"                        #############################"<<endl;
		cout<<"                        #    End of Debug Part      #"<<endl;
		cout<<"                        #############################"<<endl<<endl;
	}
	
	return jieguo;
}



void jiadizhi(char beijia[fl])
{
	string strbeijia=beijia;
	strbeijia.insert(0,Dizhi"\\");
	str2char(strbeijia,beijia);
	
}

void jiandizhi(char beijia[fl])
{
	string strbeijia=beijia;
	string diZhi=Dizhi"\\";
	int diZhiLength=diZhi.length();//地址长度
	
	strbeijia.replace(0,diZhiLength,"");
	str2char(strbeijia,beijia);
	
	if (debugMode)
	{
		cout<<"After jiandizhi:"<<endl;
		puts(beijia);
		cout<<endl;
	}
}

void unicodeInspect(char save[fn][fl],int rfn,int wrongCode[fn])
{
	for(int i=0;i<rfn;i++)
	{	
		jiadizhi(save[i]);
		//puts(save[i]);
		if(_access(save[i],0))//如果文件不存在:
			wrongCode[i]=1;//Contain unicode string
		jiandizhi(save[i]);
		//puts(save[i]);
	}
}

void standardcha(char beista[fl])
{
	string temp=beista;
	str2char(temp,beista);
}

void standardcha2(char beista[fenjiel])
{
	string temp=beista;
	str2char2(temp,beista);
}

void shuchuwenyulan(char save[fn][fl],char OT[fn][fl],int wrongCode[fn],int rfn)
{
	int successCnt=0;//成功计数
    FILE *fp = fopen("Result.txt","w");
	
	fputs("(<ゝω·)Kira~☆ Succed part\r\n----------------------------------------\r\n",fp);
    for(int i=0;i<rfn;i++)//成功的输出
    {
		if (wrongCode[i]!=0)
			continue;
		
		successCnt++;
		
		if(strcmp(save[i],OT[i])==0)//文件名已经标准化
		{
			fputs("The file: ",fp);
			fputs(save[i],fp);
			fputs("\r\nNo need to convert.\r\n\r\n",fp);
			continue;
		}
		
		fputs("Old file: ",fp);
		fputs(save[i],fp);
		
		fputs("\r\nNew file: ",fp);
		fputs(OT[i],fp);
		fputs("\r\nRenamed succed.\r\n\r\n",fp);
	}
	if (successCnt != rfn)
	{
		fputs("∑(゜ロ゜;)!! Failed part\r\n----------------------------------------\r\n",fp);
		for (i=0;i<rfn;i++)//失败的输出
		{
			if (wrongCode[i]==0)
				continue;
			
			fputs("The file: ",fp);
			fputs(save[i],fp);
			fputs("\r\n",fp);
			
			switch (wrongCode[i])
			{
			case 1:fputs("Contain unicode string.\r\n",fp);break;
			case 2:fputs("Brackets inspect might be wrong.\r\n",fp);break;
			case 3:fputs("Rename failed for invalid argument.\r\n",fp);break;
			default:fputs("Unknown erroe.\r\n",fp);
			}
			
			fputs("\r\n",fp);
		}
	}
	
	fputs("(`·ω·′) Result part\r\n----------------------------------------\r\n",fp);
	if (successCnt == rfn)
	{
		fputs("All the ",fp);
		fprintf(fp,"%d",rfn);
		fputs(" files have been renamed successed.\r\n",fp);
	}
	else
	{
		fputs("Still ",fp);
		fprintf(fp,"%d",rfn-successCnt);
		fputs(" files in ",fp);
		fprintf(fp,"%d",rfn);
		fputs(" can not be renamed.",fp);
	}
	fclose(fp);
}

void renameall(char save[fn][fl],char OT[fn][fl],int wrongCode[fn],int rfn)
{
	//viewshuzu(save);
	//viewshuzu(OT);
	
	for(int i=0;i<rfn;i++)
	{	
		if (wrongCode[i]!=0)//有错误直接跳出
			continue;
		
		jiadizhi(save[i]);
		jiadizhi(OT[i]);
		
		if(strcmp(save[i],OT[i])!=0)//原文件名没有标准化
		{		
			//viewshuzu(save);
			//viewshuzu(OT);
			
			if(_access(OT[i],0)==0)//如果标准化文件已存在:
			{
				jiandizhi(save[i]);
				jiandizhi(OT[i]);
				wrongCode[i]=4;
				continue;
			}
			
			if (!debugMode)
				if (rename(save[i], OT[i]) != 0)//重命名
				{
					perror("rename");
					wrongCode[i]=3;//非法参数
				}	
		}
		jiandizhi(save[i]);
		jiandizhi(OT[i]);
	}
}

void renameallBack(char save[fn][fl],char OT[fn][fl],int wrongCode[fn],int rfn)
{
	int successCnt=0;
	
	cout<<endl;
	cout<<"                        #############################"<<endl;
	cout<<"                        #      Rename back Part     #"<<endl;
	cout<<"                        #############################"<<endl<<endl;
	
	cout<<"_(:з」∠)_  Back part"<<endl<<"----------------------------------------";
	for(int i=0;i<rfn;i++)
	{	
		if (wrongCode[i]!=0 && wrongCode[i]!=4)//有错误直接跳出
			continue;
		
		successCnt++;
		
		jiadizhi(OT[i]);
		jiadizhi(save[i]);	
		if(_access(save[i],0)==0)//如果原文件存在:
		{
			jiandizhi(OT[i]);
			jiandizhi(save[i]);
			cout<<endl<<"The file: "<<save[i]<<endl;
			cout<<"Original file has existed."<<endl;
			continue;
		}
		
		if(strcmp(save[i],OT[i])!=0)//原文件名没有标准化
		{	
			jiandizhi(OT[i]);
			jiandizhi(save[i]);
			cout<<endl<<"New file: "<<OT[i]<<endl;
			cout<<"Ori file: "<<save[i]<<endl;
			
			//viewshuzu(save);
			//viewshuzu(OT);
			
			jiadizhi(OT[i]);
			jiadizhi(save[i]);			
			if (rename(OT[i], save[i]) == 0)//重命名
			{
				wrongCode[i]=0;
				cout<<"Renamed back succed."<<endl;
			}
		}
		jiandizhi(OT[i]);
		jiandizhi(save[i]);
	}
	
	if (successCnt==0)
		cout<<endl<<"No file need rename back."<<endl;
	
	cout<<endl<<"(`·ω·′) Result part"<<endl<<"----------------------------------------"<<endl;
	cout<<"Press Enter key to exit."<<endl<<"Press r key to rename."<<endl;
}

void epilogue(char save[fn][fl],char OT[fn][fl],int wrongCode[fn],int rfn)
{
	int successCnt=0;//成功计数
	
	cout<<"                        #############################"<<endl;
	cout<<"                        #        Rename Part        #"<<endl;
	cout<<"                        #############################"<<endl<<endl;
	
	cout<<"(<ゝω·)Kira~☆ Succed part"<<endl<<"----------------------------------------"<<endl;
	for (int i=0;i<rfn;i++)//成功的输出
	{
		if (wrongCode[i]!=0 && wrongCode[i]!=4)//除了4的其余数
			continue;
		
		successCnt++;
		
		if(strcmp(save[i],OT[i])==0)//文件名已经标准化
		{	
			cout<<"The file: "<<save[i]<<endl;
			cout<<"No need to convert."<<endl<<endl;
			continue;
		}
		
		if (wrongCode[i]==4)//标准化文件已存在
		{
			cout<<"The file: "<<OT[i]<<endl;
			cout<<"Standard file has existed."<<endl<<endl;
			continue;
		}
		
		cout<<"Old file: "<<save[i]<<endl;
		cout<<"New file: "<<OT[i]<<endl;
		cout<<"Renamed succed."<<endl<<endl;
	}
	
	if (successCnt != rfn)
	{
		cout<<"∑(゜ロ゜;)!! Failed part"<<endl<<"----------------------------------------"<<endl;
		for (i=0;i<rfn;i++)//失败的输出
		{
			if (wrongCode[i]==0 || wrongCode[i]==4)
				continue;
			
			cout<<"The file: "<<save[i]<<endl;
			
			switch (wrongCode[i])
			{
			case 1:cout<<"Contain unicode string."<<endl;break;
			case 2:cout<<"Brackets inspect might be wrong."<<endl;break;
			case 3:cout<<"Rename failed for invalid argument."<<endl;break;
			default:cout<<"Unknown erroe."<<endl;
			}
			
			cout<<endl;
		}
	}
	
	cout<<"(`·ω·′) Statistics part"<<endl<<"----------------------------------------"<<endl;
	if (successCnt == rfn)
		cout<<"All the "<<rfn<<" files have been renamed successed."<<endl;
	else
		cout<<"Still "<<rfn-successCnt<<" files in "<<rfn<<" can not be renamed."<<endl;
	
	cout<<"Press Enter key to exit."<<endl<<"Press b key to rename back."<<endl;
}

void main()
{
	char save[fn][fl];//原文件夹名
	string jieguo[fn];//处理后文件夹名
	char OT[fn][fl];//处理后文件夹名
	int rfn=0;//文件夹数量
	int wrongCode[fn]={0};//错误代码。1：括号检验错误。2：包含unicode字符
	char changeInfo[fn][fenjiel];//转换文本信息
	string changeInfoStr[fn];//转换文本信息
	int strn=radetext(changeInfo,"changeInfo.txt")/2;//转换文本信息数量
	
	
	rfn=duquwenjianming(save);//读取文件夹名
	if (debugMode)
	{
		cout<<"                        #############################"<<endl;
		cout<<"                        #        Debug Part         #"<<endl;
		cout<<"                        #############################"<<endl<<endl;
		cout<<"Reads the file name:"<<endl;
		viewshuzu(save,rfn);
	}
	
	unicodeInspect(save,rfn,wrongCode);//检验unicode字符
	//viewshuzu1(wrongCode,rfn);
	if (debugMode)
	{
		cout<<"Inspection unicode string:"<<endl;
		viewshuzu(save,rfn);
		cout<<endl;
	}
	
	for(int i=0;i<(strn+1)*2;i++)//转换信息标准化
	{
		standardcha2(changeInfo[i]);
		changeInfoStr[i]=changeInfo[i];
	}
	
	//	int sucn=0;
	for(i=0;i<rfn;i++)
	{
		jieguo[i]=fenjie(save[i],changeInfoStr,strn,&wrongCode[i]);
		str2char(jieguo[i],OT[i]);
	}
	//viewshuzu1(wrongCode,rfn);
	//cout<<sucn<<endl;
	//viewshuzu(OT,rfn);
	//cout<<viewshuzu2(jieguo)<<endl;
	
	renameall(save,OT,wrongCode,rfn);
	
	shuchuwenyulan(save,OT,wrongCode,rfn);
	
	epilogue(save,OT,wrongCode,rfn);
	
	char key;
	while (key!=13)
	{
		key=getch();
		if (key==98)
			renameallBack(save,OT,wrongCode,rfn);//返回初始状态
		if (key== 114)
		{
			cout<<endl;
			renameall(save,OT,wrongCode,rfn);
			shuchuwenyulan(save,OT,wrongCode,rfn);
			epilogue(save,OT,wrongCode,rfn);
		}
	}
}