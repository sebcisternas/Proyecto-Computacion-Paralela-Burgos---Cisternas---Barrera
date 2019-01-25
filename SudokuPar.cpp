#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <omp.h>
#include <ctime>
#include <iomanip>
using namespace std;

unsigned t0, t1;
//tomar entrada de ingreso y separar en numeros
int Cvrt(string dato)
{
	int n = atoi(dato.c_str());
    return n;
}
//verificar si es numero
bool VrfNum(string dato)
{
	if(Cvrt(dato)==0)
        {
		return true;
        }
	else
        {
		int aux = atoi(dato.c_str());
		if(aux==0)
		{
			return false;
		}
		else
		{
			return true;
		}
        }
}
//muestra Matriz actual
void Mostrar(int Matriz[][9])
{
	cout<<"---------MATRIZ----------"<<endl;
	for(int i=0;i<9;i++)
        {
		for(int j=0;j<9;j++)
		{
			cout<<Matriz[i][j]<<" ";
		}
		cout<<endl;
        }
}
//revisa la fila y verifica si existe el numero ingresado por argumento
bool RvsFila(int Matriz[][9], int num, int fila)
{
	bool exst=true;
	for(int j=0;j<9;j++)
	{
		if(Matriz[fila][j]==num)
		{
			exst=false;
		}
	}
	return exst;
}
//revisa la columna y verifica si existe el numero ingresado por argumento
bool RvsCol(int Matriz[][9], int num, int columna)
{
	bool exstp=true;
	for(int x=0;x<9;x++)
	{
		if(Matriz[x][columna]==num)
		{
			exstp=false;
		}
	}
	return exstp;
}

//revisa el area de 3x3 para verificar la no existencia del numero
bool RvsSubMat(int Matriz [][9], int fila, int columna, int num)
{
	bool ressp=true;
	int auxi1, auxi2, auxj1, auxj2;
	if(fila<3)
	{
		auxi1=0;
		auxi2=2;
	}
	if(fila>=3 && fila<6)
	{
		auxi1=3;
		auxi2=5;
	}
	if(fila>=6 && fila<9)
	{
		auxi1=6;
		auxi2=8;
	}
	if(columna<3)
	{
		auxj1=0;
		auxj2=2;
	}
	if(columna>=3 && columna<6)
	{
		auxj1=3;
		auxj2=5;
	}
	if(columna>=6 && columna<9)
	{
		auxj1=6;
		auxj2=8;
	}
	for(int y=auxi1;y<=auxi2;y++)
	{
		for(int z=auxj1;z<=auxj2;z++)
		{
			if(Matriz[y][z]==num)
			{
				ressp=false;
			}
		}
	}
	return ressp;
}

//hace el cambio en la Matriz
void RellenoMat(int Matriz[][9])
{

	bool ver1,ver2,ver3;
	int target_thread_num = 4;
	omp_set_num_threads(target_thread_num);
	unsigned long times[target_thread_num];
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			cout<<endl;
			if(Matriz[i][j]==0)
			{
				cout<<endl;
				int n=1;
				while(n<10)
				{
					cout<<endl;
					//conjunto de hilos e inicio de la ejecucion paralela
					#pragma omp parallel sections
    				{	//hilos ejecutados por equipo
						#pragma omp section
        					{	//define variable para guarda el numero del procesador
        						int hiloid = omp_get_thread_num();

   								cout << "Nmuero de hilo: " << omp_get_thread_num() << endl;
        						ver1=RvsFila(Matriz,n,i);

        					}
						#pragma omp section
        					{
        						int hiloid = omp_get_thread_num();

   								cout << "Numero de hilo: " << omp_get_thread_num() << endl;
        						ver2=RvsCol(Matriz,n,j);
        					}

						#pragma omp section
							{
								int hiloid = omp_get_thread_num();

   								cout << "Numero de hilo: " << omp_get_thread_num() << endl;
								ver3=RvsSubMat(Matriz,i,j,n);
							}
    				}
					if(ver1 && ver2 && ver3)
					{
						Matriz[i][j]=n;
						n++;
						break;
					}
					n++;
				}
			}
		}
	}
}
int main(int argc, char* argv[])
{
	bool verif3, verif2, exst, exst1, exst2, verif1;
	int coaux,fiaux;
	int subm[3][3];
	int Matriz[9][9];
	t0=clock();
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			Matriz[i][j]=0;

	string argumento=argv[1];
	string pos_i;
	string pos_j;
	string num;
	if (argumento.substr(0,1)=="[")
	{
		cout<<endl;
		//mientras argumento ingresado sea distinto de vacio
		while(argumento!="")
		{
			pos_i=argumento.substr(1,1);
			pos_j=argumento.substr(3,1);
			num=argumento.substr(5,1);
			//si se cumplen las condiciones
			if(VrfNum(pos_i) && VrfNum(pos_j) && VrfNum(num) && Cvrt(pos_i)>=0 && Cvrt(pos_i)<9 && Cvrt(pos_j)>=0 && Cvrt(pos_j)<9 && Cvrt(num)>=1 && Cvrt(num)<=9)
			{	//conjunto de hilos e inicio de la ejecucion paralela
				#pragma omp parallel sections
    			{	//hilos ejecutados por equipo
					#pragma omp section
        			{
						verif1=RvsFila(Matriz,Cvrt(num),Cvrt(pos_i));
        			}
					#pragma omp section
        			{
        				verif2=RvsCol(Matriz,Cvrt(num),Cvrt(pos_j));
        			}

					#pragma omp section
					{
						verif3=RvsSubMat(Matriz,Cvrt(pos_i),Cvrt(pos_j),Cvrt(num));
					}
    			}




				if(verif1 && verif2 && verif3)
				{
					Matriz[Cvrt(pos_i)][Cvrt(pos_j)]=Cvrt(num);
				}
			}
			else
			{	//error por falla en la verificacion
				cout<<"Error de formato"<<endl;
			}
			argumento=argumento.substr(7);
		}
		cout<<endl;
	}
	else
	{	//error por agregar 0
		cout<<"Error de formato"<<endl;
	}
	//mostrar Matriz solucion
	RellenoMat(Matriz);
	Mostrar(Matriz);
	//inicio del reloj
	t1=clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
	//creacion y apertura de archivo con solucion
	ofstream archivo;
	archivo.open("solucion.csv");
	for(int g=0;g<9;g++)
	{
		for(int h=0;h<9;h++)
		{
			if(h<8)
			{
				archivo<<Matriz[g][h]<<",";
			}
			else
			{
				archivo<<Matriz[g][h]<<"\n";
			}
		}
	}
	//cierre archivo csv
	archivo.close();
}
