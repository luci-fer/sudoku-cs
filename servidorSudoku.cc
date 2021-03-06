//  Hacer un Servidor para jugar sudoku "Fil;Col;val;id"
//TO DO: Validar, Hashtable for id score. 
// Linea de compilación: g++ -I/home/utp/zmq/include -L/home/utp/zmq/lib -o server servidorSudoku.cc -lzmq


#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <cstdint>
#include <sstream> 



#include <string>
#include <map>
#include <vector>


using namespace std;

///////////////////SCORE BOARD FUNCTIONS////////////////////////////////////////
void vectorize_hash(map<string,int>& map,vector<string>& names, vector<int>& scores)
	{

		int i = 0;
		for (auto o : map)
			{
		
				names.push_back(o.first);
				scores.push_back(o.second);
			}

	}


void print_scoreboard(vector<int> v1, vector<string> v2)
	{
		int i;

		int tam = v1.size();
		cout << "--------------------" <<endl;
		for (i = 0 ; i < tam ; i++)
			{
				cout << v2[i] << " => " << v1[i]<<endl; 
			}
		cout << "--------------------" <<endl;
	}


void  addplayer_points(string nickname,int amount,map<string,int>& map)
	{
			map[nickname] += amount;	
	}


void find_winner(int& mayor,int& index,vector<int>& vect1,vector<string>& vect2){
			mayor = vect1[0];
			index = 0;
			int tam = vect1.size();
			int i;
		for (i = 1 ; i < tam ; i++)
			{
				if(mayor > vect1[i])
					{
						mayor = mayor;
						//cout << "if" << endl;
						//cout << mayor << endl;
					}
				else
					{
						mayor = vect1[i];
						index = i;
						//cout << "else" << endl;
						//cout << mayor  << endl;
					}
			}
		//cout << "mayor:" << endl;
		//cout << "winner index:" << endl;
		//cout << index << endl;
		cout << "CURRENT SUDOKU MASTER" << endl;
		cout << vect2[index] <<": " << mayor << endl;		
		//cout << mayor << endl;
	}


///////////////////////////////////////////////////////////////////////////////



/** Check whether grid[i][j] is valid in the grid */
bool isValid(int i, int j, int grid[][9])
{
  // Check whether grid[i][j] is valid at the i's row
  for (int column = 0; column < 9; column++)
    if (column != j && grid[i][column] == grid[i][j])
      return false;

  // Check whether grid[i][j] is valid at the j's column
  for (int row = 0; row < 9; row++)
    if (row != i && grid[row][j] == grid[i][j])
      return false;

  // Check whether grid[i][j] is valid in the 3 by 3 box
  for (int row = (i / 3) * 3; row < (i / 3) * 3 + 3; row++)
    for (int col = (j / 3) * 3; col < (j / 3) * 3 + 3; col++)
      if (row != i && col != j && grid[row][col] == grid[i][j])
        return false;

  return true; // The current value at grid[i][j] is valid
}

bool verify_block(int board[9][9], unsigned blockID)
{
    unsigned digits[9] = { 0 } ;

    const unsigned rowBegin = (blockID / 3) * 3 ;
    const unsigned colBegin = (blockID % 3) * 3 ;

    for ( unsigned i=0; i<3; ++i )
        for ( unsigned j=0; j<3; ++j )
            if ( digits[board[rowBegin+i][colBegin+j]-1]++ )
                return false ;

    return true ;
}

bool subsq_verify(int board[9][9]) 
{
    bool verified = true ;
    for ( unsigned i=0; i<9; ++i )
        if ( !verify_block(board, i) )
        {
            std::cout << "Sub-region " << i+1 << " no es correcta.\n" ;
            verified = false ;
        }

    return verified ;
}




void inicializarSudoku(int matrix[9][9])
{
	int i,j;
	for (i=0; i<9; i++)
	{
		for(j=0;j<9;j++)
		{
			matrix[i][j]=0;
		}
	}
	matrix[0][2]=4;
	matrix[0][3]=3;
	matrix[0][8]=6;
	matrix[1][0]=6;
	matrix[1][3]=1;
	matrix[1][5]=9;
	matrix[2][0]=7;
	matrix[2][7]=4;
	matrix[2][8]=9;
	matrix[3][2]=1;
	matrix[3][4]=8;
	matrix[3][5]=5;
	matrix[3][6]=4;
	matrix[3][7]=6;
	matrix[4][2]=6;
	matrix[4][3]=2;
	matrix[4][4]=9;
	matrix[4][5]=3;
	matrix[4][6]=7;
	matrix[5][1]=7;
	matrix[5][2]=5;
	matrix[5][3]=4;
	matrix[5][4]=1;
	matrix[5][6]=9;
	matrix[6][0]=4;
	matrix[6][1]=6;
	matrix[6][8]=7;
	matrix[7][3]=9;
	matrix[7][5]=4;
	matrix[7][8]=1;
	matrix[8][0]=2;
	matrix[8][5]=7;
	matrix[8][6]=8;	
}

//Se debe devolver un String con la matriz impresa y enviar ese String al cliente.
void mostrar_sudoku(int matrix[9][9])
	{
		int i;
		int j;
		printf("                            ---------------------\n");
		for (i = 0; i < 9; i++)
		{
			printf("                            | ");
			for (j = 0; j < 9; j++)
			{
				printf("%d ",matrix[i][j]);
			}
			printf("|\n");
			/* code */
		}
		printf("                            ---------------------\n\n");
	}

string to_string(int matrix[9][9])
{
		stringstream stream; 

		int numero = 0; 
		string palabra; 
		int h;
		int f;
		//string cadena="
		stream << "                            ---------------------" << endl;
		for (h = 0; h < 9; h++)
		{
			stream << "                            | ";
			for (f = 0; f < 9; f++)
			{
				stream << matrix[h][f]<<" ";
				//cadena+=stream.str();
			}
			stream << "|"<<endl;
			/* code */
		}
		stream << "                            ---------------------"<<endl<<endl;
		return stream.str();
}

void burn_play(int matrix[9][9],int row, int column,int value)
	{
		matrix[row][column] = value;
	}






///matriz[fila][col]
///


int main (void)
{
	int matriz[9][9];
	inicializarSudoku(matriz);
	//cout << matriz[0][0] << endl;
	cout << to_string(matriz);
    //  Socket to talk to clients
    //192.168.9.227
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP); //LA CONSTANTE ES PARA RESPONDER SOLICITUDES
    int rc = zmq_bind (responder, "tcp://192.168.9.227:5555"); //*-> INTERFAZ DE RED POR DEFECTO
    assert (rc == 0);
    
	int id;
	int fila=0;
	int col=0;
	int valor=0;
	int score=0;
	int temp= 0;
	stringstream stream1;

		//////////////////////////CREACION DE SCOREBOARD A PARTIR DE JUGADA
		map<string,int> players;

		int mayor;
		int winner_index;
	
	
    while (1) {
		//reinicia vectores para calculo de nuevo sudoku master para. 
		vector<int> vect1;
		vector<string> vect2;
		

		//temp = matriz[fila,col];////??????????????????????????
        char buffer [100];
        printf(".............:::::::::::::::SUDOKU SERVER ONLINE:::::::::::::::............. \n");
		mostrar_sudoku(matriz);
		//cout << to_string(matriz);
        zmq_recv (responder, buffer, 100, 0);
        //printf ("%s\n",buffer);
      
        ///printf ("%s\n",op);
     
        ////JUGADA
		//
		sscanf(buffer,"%d;%d;%d;%d", &fila,&col,&valor,&id);
		printf("fila: %d\n",fila);
		printf("columna: %d\n",col);
		printf("valor: %d\n",valor);
		printf("Jugador: %d\n",id );
		//Se Almacena el valor que tiene la celda, antes de ser asignada. Para retroceder movimientos invalidos.
		//temp = matriz[fila,col];/////NO LLEGA EL VALOR QUE SE PRETENDE. Gracias jodido y complicado C :@
		valor=(int)valor;
		burn_play(matriz,fila,col,valor);

		mostrar_sudoku(matriz);

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		if(!isValid(fila,col,matriz))
		{
			burn_play(matriz,fila,col,0); //CAPTURA DEL NUMERO DESDE LA MATRIZ EN VEZ DE 0
			//stream1 << to_string(matriz); 
			zmq_send (responder, "entrada invalida", 100, 0);
			/////si la entrada es invalidase agregara el id => -1 si no existe a el mapa.
			/////si existe se le restara 1 al puntaje.
			stream1 << id;
			addplayer_points(stream1.str(),-1,players);			
			vectorize_hash(players,vect2,vect1);
			print_scoreboard(vect1,vect2);
			find_winner(mayor,winner_index,vect1,vect2);

		}else
		{	
			//si la jugada es exitosa y el id no existe se agrega id => 1 al mapa.
			//si ya existe se le sumara 1.
			stream1 << id; 
			addplayer_points(stream1.str(),1,players);

			vectorize_hash(players,vect2,vect1);
			print_scoreboard(vect1,vect2);
			find_winner(mayor,winner_index,vect1,vect2);
			
		}
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		/*if(!subsq_verify(matriz))
		{
			zmq_send (responder, "Entrada invalida", 100, 0);
			//burn_play(matriz,fila,col,temp);
		}


		/*if  (!isValid(matriz)) 
			{
				cout << "Invalid input" << endl;
				burn_play(matriz,fila,col,0);
			}*/
		//cout << isValid(matriz);
		//sleep (5);          //  Do some 'work'



        
        stream1 << to_string(matriz); 
        zmq_send (responder, "Jugada Exitosa. ", 200, 0);
        



    }
       
    return 0;
}
