
//�Զ��Ű�	Alt+F8

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
#define debugMode 0//0:�ء�1:����

int duquwenjianming(char save[fn][fl])
{
    system("dir /b "Dizhi">>buf.txt");//��Ŀ¼������buf.txt
	
    FILE *fp = fopen("buf.txt","rb");
	
	int i=0;
    while (!feof(fp))
    {		
        fgets(save[i],fl,fp);
		i++;
    }
	
    fclose(fp);
	
	system("del /Q buf.txt");  //ɾ��Ŀ¼��buf.txt�ļ�
	
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

void viewshuzu(char save[fn][fl],int strn)//�鿴�ַ�����
{
	for(int i=0;i<strn;i++)
	{
		//		cout<<"*";
		puts(save[i]); //��ʾ�ļ���
		//		cout<<"*";
	}
	//cout<<endl;
}

void viewshuzu(char save[fn][fenjiel],int strn)//�鿴�ַ�����
{
	for(int i=0;i<strn;i++)
	{
		//		cout<<"*";
		puts(save[i]); //��ʾ�ļ���
		//		cout<<"*";
	}
	//cout<<endl;
}

void viewshuzu1(int intshuzu[fenjien])//�鿴��������
{
	for(int i=0;i<fenjien;i++)
	{
		cout<<intshuzu[i]<<" ";
	}
	cout<<endl;
}

void viewshuzu1(int intshuzu[fn],int rfn)//�鿴��������
{
	for(int i=0;i<rfn;i++)
	{
		cout<<intshuzu[i]<<" ";
	}
	cout<<endl;
}

int viewshuzu2(string stringshuzu[fenjien])//�鿴String����
{
	for (int i=0;i<10;i++)
	{
		cout<<i<<"*"<<stringshuzu[i]<<"*";
		cout<<endl;
	}
	cout<<endl;
	return i;
}

void myfind(int chu[fenjien],char *zhao,string zifu)//�����ַ��к�����������λ��
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
	if(str.length()!=0)//���鲻Ϊ��
	{
		for(;i<str.length();i++)
			cha[i] = str[i];
		//		cha[i] = '\n';//ÿ�а���һ�����з� ��ֹ��
		//		cout<<cha<<endl;//���ת�����
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
	if(str.length()!=0)//���鲻Ϊ��
	{
		for(;i<str.length();i++)
			cha[i] = str[i];
		//		cha[i] = '\n';//ÿ�а���һ�����з� ��ֹ��
		//		cout<<cha<<endl;//���ת�����
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

int eareSearch(int lowEdge,int upEdge,int eare[fenjien])//����һ����Χ���Ƿ����һ�������е�����
{
	for (int i=0;i<fenjien;i++)
	{
		if (lowEdge<eare[i] && eare[i]<upEdge)
			return 1;
	}
	return 0;
}

string fenjie(char save1[fl],string changeInfoStr[fn],int strn,int* wrongCode)//�ֽ��ַ����鵽�ɷ�����ʽ
{
	string save11=save1;
	string save111[10];
	string tempstr;
	string empty;
	string jieguo;
	
	if (*wrongCode!=0)//�д���ֱ������
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
	
	//Ԥ����
	int idx=0;
	//cout<<save11<<endl;
	
	while (1)//�䡾��Ϊ[]
	{
		idx=save11.find("��");
		if (idx==-1)
			break;
		save11.replace(idx,2,"[");
	}
	
	while (1)//�䡾��Ϊ[]
	{
		idx=save11.find("��");
		if (idx==-1)
			break;
		save11.replace(idx,2,"]");
	}
	
	while (1)//�䣨��Ϊ()
	{
		idx=save11.find("��");
		if (idx==-1)
			break;
		save11.replace(idx,2,"(");
	}
	
	while (1)//�䣨��Ϊ()
	{
		idx=save11.find("��");
		if (idx==-1)
			break;
		save11.replace(idx,2,")");
	}
	
	while (1)//����~
	{
		idx=save11.find("��");
		if (idx==-1)
			break;
		save11.replace(idx,2,"~");
	}
	//	cout<<save11<<endl;
	
	idx=save11.find("[DL��]");//����[DL��]
	if (idx!=-1)
		save11.replace(idx,6,"");
	
	idx=save11.find("[�o����]");//����[�o����]
	if (idx!=-1)
		save11.replace(idx,8,"");
	
	idx=save11.find("(���ꥸ�ʥ�)");//����(���ꥸ�ʥ�)
	if (idx!=-1)
		save11.replace(idx,12,"");
	
	idx=save11.find("(���)");//����(���)
	if (idx!=-1)
		save11.replace(idx,8,"");
	
	idx=save11.find("[Chinese]");//δ֪������
	if (idx!=-1)
	{
		unknown=1;
		save11.replace(idx,9,"");
	}
	
	idx=save11.find("[�й����U]");//δ֪������
	if (idx!=-1)
	{
		unknown=1;
		save11.replace(idx,10,"");
	}
	
	idx=save11.find("��");//δ֪������
	if (idx!=-1)
	{
		unknownMark=1;
	}

	myfind(ndq,"[",save11);//����[����λ��
	myfind(ndh,"]",save11);//����]����λ��
	myfind(nxq,"(",save11);//����(����λ��
	myfind(nxh,")",save11);//����)����λ��
	
	//viewshuzu1(ndq);
	//viewshuzu1(ndh);
	//viewshuzu1(nxq);
	//viewshuzu1(nxh);	
	
	for (int i=1;i<fenjien;i++)
	{
		if ((ndq[i]*ndh[i]==0 && ndq[i]+ndh[i]!=0) || (nxq[i]*nxh[i]==0 && nxq[i]+nxh[i]!=0))//������ֻ��һ��Ϊ0
		{
			*wrongCode=2;//Brackets inspect might be wrong
			jieguo.append(save11);
			return jieguo;
		}
	}
	
	if (ndh[0]!=0 || nxh[0]!=0)//����[]��()���ļ���
	{
		int pretreatment=0;//������������()
		if (ndh[1]==0 && !eareSearch(ndq[0],ndh[0],nxh))//ֻ��һ��[]�Ҳ�����()
		{
			save11.insert(ndh[0],"()");//�����ʽ()
			pretreatment=1;
		}
		else
		{
			if (ndh[1]!=0 && !eareSearch(ndq[0],ndh[0],nxh) && !eareSearch(ndq[1],ndh[1],nxh))//��2��[]�Ҳ�����()
			{
				if (ndh[1]>save11.length()-4)//"[]"����󣬼��������ں�
				{
					save11.insert(ndh[0],"()");//�����ʽ()
					pretreatment=1;
				}
				else
				{
					save11.insert(ndh[1],"()");//�����ʽ()
					pretreatment=1;
				}
			}
		}
		
		if (pretreatment==1)
		{
			myfind(ndq,"[",save11);//����[����λ��
			myfind(ndh,"]",save11);//����]����λ��
			myfind(nxq,"(",save11);//����(����λ��
			myfind(nxh,")",save11);//����)����λ��
		}
		
		//cout<<save11<<endl;
		
		pretreatment=0;//�ļ���û����)��]��β
		if (save11.find(')',save11.length()-4)==-1 && save11.find(']',save11.length()-4)==-1)//�ļ���û����)��]��β
		{
			save11.insert(save11.length(),"()");//�����ʽ()
			pretreatment=1;
		}
		
		if (pretreatment==1)
		{
			myfind(nxq,"(",save11);//����(����λ��
			myfind(nxh,")",save11);//����)����λ��
		}
		
		//cout<<save11<<endl;
		
		pretreatment=0;//�����������ʱ��û��ԭ����
		if (save11.rfind(']')>save11.length()-4)//�����������
		{
			if (save11.rfind(')')!=save11.rfind('[')-1 && save11.rfind(')')!=save11.rfind('[')-2)//������ǰû��ԭ����
			{
				save11.insert(save11.rfind('['),"()");
				pretreatment=1;
			}
		}
		
		if (pretreatment==1)
		{
			myfind(ndq,"[",save11);//����[����λ��
			myfind(ndh,"]",save11);//����]����λ��
			myfind(nxq,"(",save11);//����(����λ��
			myfind(nxh,")",save11);//����)����λ��
		}
	}
	else//û��[]��()���ļ���ֱ�����
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
		
		cout<<"Pretreatment results:"<<endl<<save11<<endl<<endl;//Ԥ������
	}
	
	////////////////////////////////////////
	//////////      Ԥ������      //////////
	////////////////////////////////////////
	
	
	//////////δ֪��������
	if(unknown==1)//δ֪������
	{
		save111[0].append("��");
		//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
	}
	
	
	//////////��ȡ[  ( )]������������
	int authorLoc=0;
	for(i=0;ndh[i]!=0;i++)
	{
		tempstr.append(save11,ndq[i],ndh[i]-ndq[i]+1);
		temp=tempstr.find("(");//�����Ƿ���"("
		if(temp!=-1)
		{
			if(tempstr.rfind(" ",temp)!=temp-1)
			{
				tempstr.insert(temp," ");
			}
			
			save111[2].append(tempstr);
			//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
			
			tempstr=empty;
			authorLoc=i;
			break;
		}
		tempstr=empty;
	}
	
	if (debugMode)
	{		   
		cout<<"Extract author name result:"<<endl;
		viewshuzu2(save111);//��ȡ���߽��
	}
	
	
	//////////��ȡ")] ("���ַ�������Ʒ��
	int ndhfix=0,nxhfix=0;
	for(i=0;i<fenjien;i++)//Ѱ��")]"��λ��
	{
		for(int j=0;j<fenjien;j++)
		{	
			if(nxh[i]==ndh[j]-1)//����")]"��λ��
			{
				ndhfix=j;
				nxhfix=i;
				goto OT0;
			}
		}
	}
OT0:;
	int qx=0,hx=0,originWorkNameLoc=0;
	for(i=nxhfix+1;i<fenjien;i++)//������")]"��λ�ú���Ѱ����")["��������")"��β��λ��
	{
		for(int j=ndhfix+1;j<fenjien;j++)
		{		
			if (nxh[i]==ndq[j]-1 || nxh[i]==ndq[j]-2 || nxh[i]>save11.length()-4)//����")["��") ["��������")"��β
			{
				originWorkNameLoc=i;
				
				tempstr.append(save11,ndh[ndhfix]+1,nxq[i]-ndh[ndhfix]-1);
				
				while(temp=tempstr.find(" ")==0)//������λ�Ƿ���" "
				{
					tempstr=empty;
					qx++;
					tempstr.append(save11,ndh[ndhfix]+1+qx,nxq[i]-ndh[ndhfix]-1-qx);
				}
				
				
				while(temp=tempstr.rfind(" ")==tempstr.size()-1)//����ĩλ�Ƿ���" "
				{
					tempstr=empty;
					hx++;
					tempstr.append(save11,ndh[ndhfix]+1+qx,nxq[i]-ndh[ndhfix]-1-qx-hx);
				}
				
				//cout<<"*"<<tempstr<<"*"<<endl;
				
				save111[4].append(tempstr);
				//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
				
				tempstr=empty;
				goto OT1;
				
			}			
		}
	}
OT1:;
	
	if (debugMode)
	{		   
		cout<<"Extract book name results:"<<endl;
		viewshuzu2(save111);//��ȡ���������
	}
	

	//////////��ȡԭ����
	int changed=0;//ת�����
	for(i=0;i<(strn+1)*2;i++,i++)
	{
		if(save11.rfind(changeInfoStr[i])!=-1)//����Ҫת������Ʒ
		{
			save111[6].append(changeInfoStr[i+1]);
			//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
			
			changed=1;
			break;
		}
	}
	if (changed==0)//û����Ҫת����
	{
		save111[6].append(save11,nxq[originWorkNameLoc],nxh[originWorkNameLoc]-nxq[originWorkNameLoc]+1);
		//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
	}
	
	if (debugMode)
	{		   
		cout<<"Extract original name result:"<<endl;
		viewshuzu2(save111);//��ȡԭ�������
	}
	

	//////////��ȡ������Ϣ
	if(authorLoc==1)//��������Ϣ����λ
	{
		save111[8].append(save11,ndq[0],ndh[0]-ndq[0]+1);//�Ƶ�ĩβ
		//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
	}
	else//��������Ϣ������λ
	{
		if (ndh[1]!=0)//�г���2��������
		{
			for (i=1;i<fenjien;i++)
			{
				if(ndh[i+1]==0)//��������Ϣ��ĩλ
				{
					save111[8].append(save11,ndq[i],ndh[i]-ndq[i]+1);//�Ƶ�ĩβ
					//0-1:��� 2-3:���� 4-5:��Ʒ�� 6-7:ԭ�� 8-9:������
					break;
				}
			}
		}
	}
	
	if (save111[8].empty() && unknownMark==1)
	{
		save111[0].append("��");//δ��ȫ������Ϣ
	}

	if (debugMode)
	{
		cout<<"Decomposition results:"<<endl;
		viewshuzu2(save111);//���н��
	}
	

	//////////ƴ��������Ϣ
	for(i=0;i<10;i++)
	{
		if (i==0)
		{
			idx=save111[i].find("()");//ȥ����ʽ()
			if (idx!=-1)
			{
				if (save111[i].length()==2)//"()"����Ϊһ��
					save111[i].erase(idx,2);
				else//"()"����������
					save111[i].erase(idx-1,3);
			}
		}
		idx=save111[i+1].find("()");//ȥ����ʽ()
		if (idx!=-1)
		{
			if (save111[i+1].length()==2)//"()"����Ϊһ��
				save111[i+1].erase(idx,2);
			else//"()"����������
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
	int diZhiLength=diZhi.length();//��ַ����
	
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
		if(_access(save[i],0))//����ļ�������:
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
	int successCnt=0;//�ɹ�����
    FILE *fp = fopen("Result.txt","w");
	
	fputs("(<�f�ء�)Kira~�� Succed part\r\n----------------------------------------\r\n",fp);
    for(int i=0;i<rfn;i++)//�ɹ������
    {
		if (wrongCode[i]!=0)
			continue;
		
		successCnt++;
		
		if(strcmp(save[i],OT[i])==0)//�ļ����Ѿ���׼��
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
		fputs("��(�b��b;)!! Failed part\r\n----------------------------------------\r\n",fp);
		for (i=0;i<rfn;i++)//ʧ�ܵ����
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
	
	fputs("(`���ء���) Result part\r\n----------------------------------------\r\n",fp);
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
		if (wrongCode[i]!=0)//�д���ֱ������
			continue;
		
		jiadizhi(save[i]);
		jiadizhi(OT[i]);
		
		if(strcmp(save[i],OT[i])!=0)//ԭ�ļ���û�б�׼��
		{		
			//viewshuzu(save);
			//viewshuzu(OT);
			
			if(_access(OT[i],0)==0)//�����׼���ļ��Ѵ���:
			{
				jiandizhi(save[i]);
				jiandizhi(OT[i]);
				wrongCode[i]=4;
				continue;
			}
			
			if (!debugMode)
				if (rename(save[i], OT[i]) != 0)//������
				{
					perror("rename");
					wrongCode[i]=3;//�Ƿ�����
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
	
	cout<<"_(:�١���)_  Back part"<<endl<<"----------------------------------------";
	for(int i=0;i<rfn;i++)
	{	
		if (wrongCode[i]!=0 && wrongCode[i]!=4)//�д���ֱ������
			continue;
		
		successCnt++;
		
		jiadizhi(OT[i]);
		jiadizhi(save[i]);	
		if(_access(save[i],0)==0)//���ԭ�ļ�����:
		{
			jiandizhi(OT[i]);
			jiandizhi(save[i]);
			cout<<endl<<"The file: "<<save[i]<<endl;
			cout<<"Original file has existed."<<endl;
			continue;
		}
		
		if(strcmp(save[i],OT[i])!=0)//ԭ�ļ���û�б�׼��
		{	
			jiandizhi(OT[i]);
			jiandizhi(save[i]);
			cout<<endl<<"New file: "<<OT[i]<<endl;
			cout<<"Ori file: "<<save[i]<<endl;
			
			//viewshuzu(save);
			//viewshuzu(OT);
			
			jiadizhi(OT[i]);
			jiadizhi(save[i]);			
			if (rename(OT[i], save[i]) == 0)//������
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
	
	cout<<endl<<"(`���ء���) Result part"<<endl<<"----------------------------------------"<<endl;
	cout<<"Press Enter key to exit."<<endl<<"Press r key to rename."<<endl;
}

void epilogue(char save[fn][fl],char OT[fn][fl],int wrongCode[fn],int rfn)
{
	int successCnt=0;//�ɹ�����
	
	cout<<"                        #############################"<<endl;
	cout<<"                        #        Rename Part        #"<<endl;
	cout<<"                        #############################"<<endl<<endl;
	
	cout<<"(<�f�ء�)Kira~�� Succed part"<<endl<<"----------------------------------------"<<endl;
	for (int i=0;i<rfn;i++)//�ɹ������
	{
		if (wrongCode[i]!=0 && wrongCode[i]!=4)//����4��������
			continue;
		
		successCnt++;
		
		if(strcmp(save[i],OT[i])==0)//�ļ����Ѿ���׼��
		{	
			cout<<"The file: "<<save[i]<<endl;
			cout<<"No need to convert."<<endl<<endl;
			continue;
		}
		
		if (wrongCode[i]==4)//��׼���ļ��Ѵ���
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
		cout<<"��(�b��b;)!! Failed part"<<endl<<"----------------------------------------"<<endl;
		for (i=0;i<rfn;i++)//ʧ�ܵ����
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
	
	cout<<"(`���ء���) Statistics part"<<endl<<"----------------------------------------"<<endl;
	if (successCnt == rfn)
		cout<<"All the "<<rfn<<" files have been renamed successed."<<endl;
	else
		cout<<"Still "<<rfn-successCnt<<" files in "<<rfn<<" can not be renamed."<<endl;
	
	cout<<"Press Enter key to exit."<<endl<<"Press b key to rename back."<<endl;
}

void main()
{
	char save[fn][fl];//ԭ�ļ�����
	string jieguo[fn];//������ļ�����
	char OT[fn][fl];//������ļ�����
	int rfn=0;//�ļ�������
	int wrongCode[fn]={0};//������롣1�����ż������2������unicode�ַ�
	char changeInfo[fn][fenjiel];//ת���ı���Ϣ
	string changeInfoStr[fn];//ת���ı���Ϣ
	int strn=radetext(changeInfo,"changeInfo.txt")/2;//ת���ı���Ϣ����
	
	
	rfn=duquwenjianming(save);//��ȡ�ļ�����
	if (debugMode)
	{
		cout<<"                        #############################"<<endl;
		cout<<"                        #        Debug Part         #"<<endl;
		cout<<"                        #############################"<<endl<<endl;
		cout<<"Reads the file name:"<<endl;
		viewshuzu(save,rfn);
	}
	
	unicodeInspect(save,rfn,wrongCode);//����unicode�ַ�
	//viewshuzu1(wrongCode,rfn);
	if (debugMode)
	{
		cout<<"Inspection unicode string:"<<endl;
		viewshuzu(save,rfn);
		cout<<endl;
	}
	
	for(int i=0;i<(strn+1)*2;i++)//ת����Ϣ��׼��
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
			renameallBack(save,OT,wrongCode,rfn);//���س�ʼ״̬
		if (key== 114)
		{
			cout<<endl;
			renameall(save,OT,wrongCode,rfn);
			shuchuwenyulan(save,OT,wrongCode,rfn);
			epilogue(save,OT,wrongCode,rfn);
		}
	}
}