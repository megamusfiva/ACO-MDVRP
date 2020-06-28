#include <iostream.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip.h>
#include <fstream.h>

ofstream MDVRP;

//Parameter MDVRP
int iterasi=0,terbaek,minn,s,rute,q,w,z,m,n,d,i,i1,i2,i3,i4,i5,j,j1,k,l,awall=0,bantukend,js,ls,iteraco,kapkendsem,kend,pilihan,pilihan1,OUTPUT;
int pindah,benar,ada=0,totpermin,loop,kapdep1,kapkend1,maxkend1,iter1,jml_pelanggan,jml_depot;
int konst[100],eval[100],sabar[300][300],group[300],banyakp[300],kaprute[300],banyakrute[300], no[300], awal=0;
int nomor[300][300],ruteee[300],rutee[300],nn[300],pdep[300][300],rutekend[300][300];
double sum[300],dxy[300][300],data[300][300],kapdep,kapkend,maxkend,iter;
char file[100],nama[100],namafile[100],lagi;
float MatFeromon[300][300],mjarak[300][300],mjarakk[300][300][300];
float sigma[300][300],newf[10][10],delta[10][10],num[10][10];
int ft[300],sumii,sumi;

int kol1,sem,lob,k1,k2,a,b,eb1[300][300],bsem[300];
double kol[300],nilai;
double ft1[300],ft2[300],ftt[300],fit[300],prob[300][300],probkum[300][300];
double eb[300][300],eba[300],ebb[300][300],tl[300];
double alpha,beta,gamma;

tabelawal (int pilih);
void aco(int banyakp[300]);
kedepot2();
void fungsitujuan(int banyakp[300]);
updateferomon();

int main ()
{
	srand(time(0));
	ulang:
	clrscr();
	cout<<"\t        Penerapan Algoritma Ant Colony Optimization"<<endl;
	cout<<"\t Untuk menyelesaikan Multi-Depot Vehicle Routing Problem"<<endl;
	cout<<"\t ========================================================"<<endl<<endl;
	cout<<"\n Sampel Data Yang Tersedia : ";
	cout<<"\n 1. Data Ukuran Kecil";
	cout<<"\n 2. Data Ukuran Sedang";
	cout<<"\n 3. Data Ukuran Besar";
	cout<<"\n 4. Data Manual";
	cout<<endl;

ifstream DataMDVRP;
cout<<"\n Masukkan Pilihan Sampel Data yang di inginkan (1/2/3/4) : ";
cin>>pilihan1;
cout<<"\n 	";
pilihan=pilihan1;
switch(pilihan)
{
	default :
	cout<<"\nPilihan yang anda masukkan salah"<<endl;
	cout<<"Apakah anda ingin mencoba lagi (Y/T)? ";
	cin>>lagi;
   if(lagi=='Y'||lagi=='y')
		goto ulang;
	else
		exit(0);
   break;
	case 1: DataMDVRP.open("DataD01.txt"); jml_pelanggan=50;jml_depot=4;
	break;
	case 2: DataMDVRP.open("DataD02.txt"); jml_pelanggan=75;jml_depot=5;
	break;
	case 3: DataMDVRP.open("DataD03.txt"); jml_pelanggan=99;jml_depot=2;
	break;
	case 4: DataMDVRP.open("DataD01M.txt");jml_pelanggan=17;jml_depot=2;
	break;
}
n=jml_pelanggan+jml_depot;
d=jml_depot;
m=jml_pelanggan;

for(i=0;i<n;i++)
{
	for(j=0;j<3;j++)
	{
		DataMDVRP>>data[i][j];
	}
}
DataMDVRP.close();

cout<<"\n Penyimpanan output pada notepad : ";
cout<<"\n   Nama file	: ";
cin>>file;
cout<<"	Nomer urut file : ";
cin>>OUTPUT;

//output notepad
strcat(file," ke-");		//menambah karakter kedalam variabel
strcpy(namafile,file);//maksudnya adalah mengcopy isi file ke namafile
itoa(OUTPUT,nama,10);//misal diinputkan hasil=1234, membuat nama berisi 1234
strcat(namafile,nama);//menambah karakter nama kedalam variabel namafile
strcat(namafile,".txt");//strcat berfungsi untuk menggabungkan 2 buah string (melakukan penggabungan variabel namafile dan.txt)
MDVRP.open(namafile);
//Input parameter MDVRP
cout<<"\n>> Input parameter MDVRP"<<endl;
do
{
	cout<<"   -> Kapasitas depot	: ";
	cin>>kapdep;
   kapdep1=kapdep;
	if(kapdep<1||kapdep!=kapdep1)
	{
		cout<<"	Inputan salah/harus integer, mohon ulangi!"<<endl;
	}
}
while(kapdep<1||kapdep!=kapdep1);

//Menghitung antar jarak DEPOT KE PELANGGAN
for (i=0;i<n;i++)
{
	for (j=0;j<n;j++)
	{
		dxy[i][j]=sqrt(pow((data[i][0]-data[j][0]),2)+pow((data[i][1]-data[j][1]),2));
	}
}
MDVRP<<"\n-> Daftar jarak depot dengan masing masing pelanggan : "<<endl;
MDVRP<<" 	"<<endl;
for (i=d;i<n;i++)
{
	for (j=0;j<d;j++)
	{
		MDVRP<<" Jarak dari "<<i<<" ke "<<j<<" : "<<dxy[i][j]<<endl;
	}
}

//Pengelompokan pelanggan
for(i=d;i<n;i++)
{
	group[i]=0;
	for(j=1;j<d;j++)
	{
		if(dxy[i][j]<dxy[i][group[i]])
		{
			group[i]=j;
		}
	}
}
MDVRP<<"\n-> Pengelompokkan pelanggan sementara : "<<endl;
MDVRP<<" 	"<<endl;
for(i=d;i<n;i++)
{
	MDVRP<<" Pelanggan ke "<<i<<" ditempatkan di depot : "<<group[i];
	MDVRP<<endl;
}

//cek hasil grouping
for(i=0;i<d;i++)
{ 	k=0;
	for(j=d;j<n;j++)
	{
		if(group[j]==i)
		{
			pdep[i][k]=j;
			k++;
		}
	}
	banyakp[i]=k;
}
MDVRP<<"\n-> Pengelompokan pelanggan sementara : "<<endl;
MDVRP<<" 	"<<endl;
for(i=0;i<d;i++)
{
	MDVRP<<"Untuk depot "<<i<<" : ";
	for(j=0;j<=banyakp[i]-1;j++)
	{
		MDVRP<<pdep[i][j]<<" ";
	}
   MDVRP<<endl;
}
MDVRP<<" 	"<<endl;
MDVRP<<endl;
MDVRP<<"-> Total permintaan masing masing depot sementara : "<<endl;
MDVRP<<" 	"<<endl;
loop=0;
do
{
	benar=0;
	loop++;
	for(i=0;i<d;i++)
	{
		totpermin=0;
		for(j=0;j<=banyakp[i]-1;j++)
		{
			totpermin=totpermin+data[pdep[i][j]][2];
		}
		MDVRP<<"-> Total permintaan depot : "<<i<<" : "<<totpermin;
		MDVRP<<endl;
   	if(totpermin>kapdep)
		{
			//menentukan yang akan dipindah
			pindah=pdep[i][0];
			for(j=1;j<=banyakp[i]-1;j++)
			{
				if(dxy[i][pindah]<dxy[i][pdep[i][j]])
  				{
					pindah=pdep[i][j];
				}
			}
			MDVRP<<"-> Pelanggan yg akan dipindah dari depot "<<i<<" : "<<pindah;
			MDVRP<<endl;
			//mencari depot terdekat untuk yang akan dipindah
         for(j=0;j<d;j++)
			{
				if(j!=i)
				{
					l=j;
               break;
				}
			}
			group[pindah]=l;
			for(j=0;j<d;j++)
			{
				if(j!=i)
				{
					if(dxy[pindah][j]<dxy[pindah][group[pindah]])
               {
						group[pindah]=j;
					}
				}
			}
      }
	else
	{
		benar++ ;
	}
}
for(i=0;i<d;i++)
{
	k=0;
	for(j=d;j<n;j++)
	{
		if(group[j]==i)
		{
			pdep[i][k]=j; k++;
		}
	}
	banyakp[i]=k;
}
MDVRP<<" 	"<<endl;
if(loop==10)
{
	break;
	MDVRP<<"Data yang anda inputkan tak bisa diolah, silahkan inputkan lagi !!";
	MDVRP<<endl;
	goto ulang;
}
}
while(benar!=d);

MDVRP<<"\n-> Pengelompokkan pelanggan akhir : "<<endl;
MDVRP<<" 	"<<endl;
for(i=0;i<d;i++)
{
	MDVRP<<"-> Untuk depot "<<i<<" : ";
	for(j=0;j<=banyakp[i]-1;j++)
	{
		MDVRP<<pdep[i][j]<<" ";
	}
	MDVRP<<"\n-> Banyak pelanggan di depot "<<i<<" : "<<banyakp[i];
	MDVRP<<endl;
}
MDVRP<<" 	"<<endl;
MDVRP<<"\n-> Total permintaan akhir masing masing depot : "<<endl;
MDVRP<<" 	"<<endl;
for(j=0;j<d;j++)
{
	totpermin=0; for(i=0;i<=banyakp[j]-1;i++)
	{
		totpermin=totpermin+data[pdep[j][i]][2];
	}
	MDVRP<<"-> Total permintaan depot : "<<j<<" : "<<totpermin<<endl;
}
MDVRP<<" 	"<<endl;
MDVRP<<endl<<endl<<endl;
do
{
	cout<<" -> Kapasitas kendaraan : ";
	cin>>kapkend;
	kapkend1=kapkend;
	if(kapkend<1||kapkend!=kapkend1)
   {
		cout<<"	Inputan salah/harus integer, mohon ulangi!"<<endl;
	}
}
while(kapkend<1||kapkend!=kapkend1);

do
{
	cout<<" -> Jumlah kendaraan : ";
	cin>>maxkend;
   maxkend1=maxkend;
	if(maxkend<1||maxkend!=maxkend1)
	{
		cout<<"	Inputan salah/harus integer, mohon ulangi! "<<endl;
	}
}
while(maxkend<1||maxkend!=maxkend1);


for(i=0;i<d;i++)
{
   kol[i]=banyakp[i];
}

do
{
	cout<<" -> Nilai alpha : ";
	cin>>alpha;
	if(alpha<0)
	{
		cout<<"	Inputan salah, mohon ulangi! "<<endl;
	}
}
while(alpha<0);

do
{
	cout<<" -> Nilai beta : ";
	cin>>beta;
	if(beta<0)
	{
		cout<<"	Inputan salah, mohon ulangi! "<<endl;
	}
}
while(beta<0);

do
{
	cout<<" -> Penguapan Feromon (0-1) : ";
	cin>>gamma;
	if(gamma<0||gamma>1)
	{
		cout<<"	Inputan salah, mohon ulangi! "<<endl;
	}
}
while(gamma<0||gamma>1);

do
{
	cout<<">> Maksimum Iterasi (1-1000) : ";
	cin>>iter;
	iter1=iter;
	if(iter1!=iter)
	{
		cout<<" Inputan harus integer, mohon ulangi!"<<endl;
	}
	if(iter<=0)
	{
		cout<<" Inputan salah, mohon ulangi!"<<endl;
	}
}
while(iter<=0||iter1!=iter);
cout<<"Output secara lengkap dapat dilihat pada file => "<<namafile<<endl;

iteraco=iter;

MDVRP<<"\n===================================";
MDVRP<<"\n//PROSES ALGORITMA ACO //";
MDVRP<<"\n==================================="<<endl;
MDVRP<<"\nParameter yang digunakan : ";
MDVRP<<"\n 1. Kapasitas Depot       : "<<kapdep;
MDVRP<<"\n 2. Kapasitas Kendaraan   : "<<kapkend;
MDVRP<<"\n 3. Jumlah Kendaraan      : "<<maxkend;
for(i=0;i<d;i++)
{
	MDVRP<<"\n 4. Jumlah Koloni Semut   : "<<kol[i];
}
MDVRP<<"\n 5. Alpha                 : "<<alpha;
MDVRP<<"\n 6. Beta                  : "<<beta;
MDVRP<<"\n 7. Penguapan feromon     : "<<gamma;
MDVRP<<"\n 8. Maximum Iterasi       : "<<iter;
MDVRP<<endl<<"==============================================================="<<endl<<endl;
//===============================================================================
k=0;
awall=0;
awal=0;
for (i5=0;i5<iteraco;i5++)
	{
      for(z=0;z<d;z++)
   	{	if(i5!=0)
			{	updateferomon();}
      	MDVRP<<"Depot ke- "<<z<<endl;
      	tabelawal(z);
         if(z==0)
      	{
         	do
      		{
         		aco(banyakp);
               awal=awal+1 ;
      		}while (awal<banyakp[0]-1);
           fungsitujuan(banyakp);
         }
         else if(z!=d)
         {
         	do
      		{
         		aco(banyakp);
            	awal=awal+1 ;
      		}while (awal<banyakp[1]-1);
           fungsitujuan(banyakp);
      	}
         kedepot2();
      MDVRP<<endl<<"==============================================================="<<endl<<endl;
      }
   }

getch();
}

 tabelawal (int pilih)
{
	MDVRP<<"\n-> Bangkit Rute Awal : "<<endl;
   k1=0;
   for(j=0;j<banyakp[pilih];j++)
	{
		k2=pdep[pilih][k1];
      for(i=0;i<banyakp[pilih];i++)
		{
			if(eb[pilih][j]>eb[pilih][i])
			{
				k2=pdep[pilih][k1];
			}
			eb1[0][j]=k2;
      }
      MDVRP<<"semut "<<j+1<<" = "<<eb1[0][j]<<endl;
      k1++;
	}
   for(i=0;i<banyakp[pilih];i++)
   	{
      	no[i]=eb1[0][i];
         //MDVRP<<no[i]<<"  ";
   	}
	MDVRP<<"\n-> Feromon : "<<endl;
	for(i=0;i<banyakp[pilih];i++)
   {
   	for(j=0;j<banyakp[pilih];j++)
      {
      	if(i!=j)
         { 	MatFeromon[i][j]=1/kol[z]; }
         else
         { MatFeromon[i][j]=0; }
         MDVRP<<setprecision(3)<<MatFeromon[i][j]<<"\t\t";
      }
      MDVRP<<endl;
   }
    MDVRP<<endl;
   MDVRP<<"\n-> Tabel Jarak Rute Awal : "<<endl;
    for (q=0;q<banyakp[z];q++)
	{
		for (w=0;w<banyakp[z];w++)
		{
		 	mjarak[q][w]=sqrt(pow((data[eb1[0][w]][0]-data[eb1[0][q]][0]),2)+pow((data[eb1[0][w]][1]-data[eb1[0][q]][1]),2));
        	MDVRP<<setprecision(2)<<mjarak[q][w]<<"\t ";
      }
      MDVRP<<endl;
   }

};
void aco (int banyakp[300])
{
	MDVRP<<"\n-> Tabel Sigma Torsi : "<<endl;
	for (i=0;i<banyakp[z];i++)
	{  sum[i]=0;
		for (j=0;j<banyakp[z];j++)
		{
      	if(mjarak[i][j]==0)
         {
         	sigma[i][j]=0;
         }
         else if(mjarak[i][j]!=0)
         {sigma[i][j]=(pow((MatFeromon[i][j]),alpha))/(pow((mjarak[i][j]),beta)); }
			sum[i]=sum[i]+sigma[i][j];
         MDVRP<<setprecision(2)<<sigma[i][j]<<"\t\t";
		}
       MDVRP<<sum[i]<<endl;
   }
	MDVRP<<"\n-> Tabel Probabilitas : "<<endl;
	for (i=0;i<banyakp[z];i++)
	{
		for (j=0;j<banyakp[z];j++)
		{
         prob[i][j]=sigma[i][j]/sum[i];
         MDVRP<<setprecision(2)<<prob[i][j]<<"\t ";
		}
       MDVRP<<endl;
	}
	MDVRP<<"\n-> Tabel Probabilitas Kumulatif: "<<endl;
	for (i=0;i<banyakp[z];i++)
	{
		for (j=0;j<banyakp[z];j++)
		{  probkum[i][0]=prob[i][0];
         if(j!=0)
         {	probkum[i][j]=prob[i][j]+probkum[i][j-1];
         }
      MDVRP<<setw(10)<<probkum[i][j];
      }

      MDVRP<<endl;
	}
	MDVRP<<"-> Random : "<<endl;
   	for (j=0;j<banyakp[z];j++)
      {
				nilai=rand()%10000+2;
         	nilai=nilai;
         	eba[j]=nilai/10000;
         	MDVRP<<"     "<<eba[j]<<endl;
      }
      MDVRP<<endl;
  MDVRP<<"\n-> Rute selanjutnya : "<<endl;
	for (j=0;j<banyakp[z];j++)
	{
		for (i=0;i<banyakp[z];i++)
		{
         if(eba[j]<probkum[j][0])
			{	rutee[j]=no[0];
      		ruteee[j]=i;}
         if(probkum[j][i]<eba[j] && probkum[j][i+1]>eba[j])
			{	rutee[j]=no[i+1];
      		ruteee[j]=i+1;}
		}
      MDVRP<<"  "<<rutee[j]<<endl;
	}
	MDVRP<<"\n-> Tabu List : "<<endl;
   	for(s=0;s<banyakp[z];s++)
		{
      	for(rute=0;rute<=awal+1;rute++)
			{sabar[s][0]=eb1[0][s];
         sabar[s][awal+1]=rutee[s];}
      }
		for(s=0;s<banyakp[z];s++)
		{
      	for(rute=0;rute<=awal+1;rute++)
			{
         	MDVRP<<sabar[s][rute]<<" ";
         }
		  //	nodeakhir[s]=Tabu[s][awal+1];
			MDVRP<<endl;
      }
      for(j=0;j<banyakp[z];j++)
		{
   	for(rute=0;rute<=awal+1;rute++)
      {  eb1[awal+1][j]=sabar[j][rute];
         //MDVRP<<eb1[rute][j]<<" ";
      }
      //MDVRP<<endl;
   }
   MDVRP<<"\n-> Tabel Jarak baru : "<<endl;
   for (q=0;q<banyakp[z];q++)
	{
		for (w=0;w<banyakp[z];w++)
		{
         for(rute=0;rute<=awal+1;rute++)
      	{
   		if(eb1[0][w]!=sabar[q][rute])
   		{
   			mjarakk[rute][q][w]=mjarak[q][w];
		   }
   		else if(eb1[0][w]==sabar[q][rute])
   		{	mjarakk[rute][q][w]=0;}

      	}
          MDVRP<<setprecision(2)<<mjarakk[awal+1][q][w]<<"\t ";
      }
      MDVRP<<endl;
   }
    //coba
    for (q=0;q<banyakp[z];q++)
	{
		for (w=0;w<banyakp[z];w++)
		{
         for(rute=0;rute<=awal+1;rute++)
      	{
             mjarak[q][w]=mjarakk[awal+1][q][w];
             mjarak[q][w]= mjarak[q][w];
         }
      }
   }

};
kedepot2()
{
	return(awal=0);
   return(banyakp[1]);
   return(bantukend=0);
   return(kend=1);
}

void fungsitujuan(int banyakp[300])
{
	MDVRP<<"-> Nilai Fungsi Tujuan : "<<endl;
	MDVRP<<" 	"<<endl;
   for(i=0;i<banyakp[z];i++)
	{
		kend=1;
      rute=1;
		sumi=dxy[z][eb1[0][i]];
      kapkendsem=data[eb1[0][i]][2];
      rutekend[kend][rute]=z;rute++; banyakrute[kend]=1;
		rutekend[kend][rute]=eb1[0][i]; rute++; banyakrute[kend]++;
      for(j=0;j<banyakp[z]-1;j++)
		{
			kapkendsem = kapkendsem + data[eb1[j+1][i]][2];
         if (kapkendsem < kapkend)
			{
				sumi=sumi+dxy[eb1[j][i]][eb1[j+1][i]];
            kaprute[kend]=kapkendsem;
				rutekend[kend][rute]=eb1[j+1][i];rute++;banyakrute[kend]++;
			}
			else
         {
				rutekend[kend][rute]=z;banyakrute[kend]++;
            kend++;
				rute=1;
				rutekend[kend][rute]=z;rute++;banyakrute[kend]=1;
            rutekend[kend][rute]=eb1[j+1][i];rute++;banyakrute[kend]++;
            sumi=sumi+dxy[eb1[j][i]][z];
				sumi=sumi+dxy[z][eb1[j+1][i]];
				kapkendsem=data[eb1[j+1][i]][2];
            if (j==banyakp[z]-2)
				{
					kaprute[kend]=kapkendsem;
				}
      	}
		}
      ft[i]=sumi+dxy[eb1[i][banyakp[z]-1]][z];
      rutekend[kend][rute]=z;
      banyakrute[kend]++;
      MDVRP<<"Fungsi Tujuan = "<<ft[i]<<endl;
		bantukend=0;
		for (i1=1;i1<=kend;i1++)
		{
			bantukend++;
			if (bantukend<=maxkend)
			{
				MDVRP<<"Kapasitas rute "<<i+1<<" : "<<kaprute[i1]<<endl;
         	MDVRP<<"Rute "<<i+1<<" : "<<endl;
				for (j1=1;j1<=banyakrute[i1];j1++)
				{
					if (j1!=banyakrute[i1])
            	{
               	MDVRP<<rutekend[i1][j1]<<"-";
               }
					else
               {	MDVRP<<rutekend[i1][j1];
               }
            }
         }
         MDVRP<<endl;
		}
      MDVRP<<endl;
   }
   // FT terbaik
   minn=10000000;
   for(q=0;q<banyakp[z];q++)
	{
  		if(minn>ft[q])//pemilihan nilai terkecil
      {minn=ft[q];
      terbaek=q;}
   }
   MDVRP<<"-> Jarak Terbaik adalah "<<minn<<endl;
   MDVRP<<"   Dari Rute ke  "<<terbaek+1<<endl;
} ;

updateferomon()
{
	for(s=0;s<banyakp[z];s++)
	{
   	eval[s]=0;
		konst[s]=0;
   }
   for(s=0;s<banyakp[z];s++)
	{
   	for(j=1;j<banyakp[z];j++)
		{
      	eval[s]=konst[s]+mjarak[eb1[j][s]][eb1[j+1][s]];
			konst[s]=eval[s];
      }
   }
	for(i=0;i<banyakp[z];i++)
	{
   	for(j=0;j<banyakp[z];j++)
		{
      	for(s=0;s<banyakp[z];s++)
			{
         	for(q=1;q<banyakp[z];q++)
				{
            	if(eb1[q][s]==i && eb1[q+1][s]==j)
               {
               	delta[i][j]=num[i][j]+(1/(eval[s]));
                  num[i][j]=delta[i][j];
               }
            }
         }
      }
		newf[i][j]=(gamma*MatFeromon[i][j])+delta[i][j];
	}
   for(i=0;i<banyakp[z];i++)
	{
   	for(j=0;j<banyakp[z];j++)
		{
      	MatFeromon[i][j]=newf[i][j];
      }
   }
};

