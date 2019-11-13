#include "matriz_entrenamiento.h"
float datos_prueba[5]={5.5,2.4,3.8,1.1,2};
int respuesta;
void setup() {
Serial.begin(9600);


}

void loop() {
 respuesta=knn(3,3,5,120);
 Serial.println("La etiqueta es:");
 Serial.println(respuesta);
 delay (5000);

}
int knn(int k,int etiquetas,int tam_fil, int tam_col){  //int retorna datos- void el metodo no retorna 
  int col;
  int fil;
  int i=0;
  int j=0;
  int etiqueta;
  float aux;
  float aux_eti;
  float sumatoria=0; //sumatoria de la elevacion al cuadrado de cada columna
  float distancia=0; //raiz de sumatoria
  
  /*matriz de k-vecinos
   * /1/2/3/ -> define numeros de vecinos
   * /2/1/1/ -> asignacion del vecino por etiqueta
   * /0.1/0.2/0.3/-> distancia 
   */
  float matriz_k [3][k];
  /* etiquetas
   *  /1/2/3/ -> etiquetas que existe dentro de la base de datos
   *  /2/1/0/ -> conteo de etiquetas dentro de k -vecinos
   */
  int matriz_etiq[2][etiquetas];
  //asignar numero de vecinos y enceramos asignacion de etiqueta 

  for(;i<k;i++){
    matriz_k[0][i]=i+1;
    matriz_k[1][i]=0;
    matriz_k[2][i]=3000+i;
  //  Serial.println(matriz_k[2][i]);
    /*
     * /1/2/3/
     * /0/0/0/
     * /3001/3002/3003/
     */
  }
  i=0;
  //asignar etiquetas y enceramos su contador 
  for (i=0;i<etiquetas;i++){
    matriz_etiq[0][i]=i+1;
    matriz_etiq[1][i]=0;
    /*
     * /1/2/2/
     * /0/0/0/
     */
  }
  i=0;
  /*PASOS PARA K-NN
   * Leer cada fila de la matriz de entrenamiento 
   * distancia entre la fila de la matriz con la nueva instancia(vector de prueba)
   * ordenar la matriz matriz_k (determinar los k vecinos de menor distancia)
   * contar las etiquetas en la matriz k y asignar en matriz etiquetas
   * elijo al mayor numero de matriz etiquetas  
   * retorno la etiqueta resultante 
   * for lento de filas, for rapido de columnas 
   */
  for (fil=0;fil<tam_fil;fil++)
  {
    for (col=0;col<tam_col-1;col++){ //-1 por que ultima columna es etiqueta 
      sumatoria=sumatoria+pow(matriz[fil][col]-datos_prueba[col],2);
    }
    distancia=sqrt(sumatoria);
    //comparacion de nuevo dato con solo la distancia mayor almacenada en matriz_k
      if(distancia<matriz_k[2][k-1]){
        matriz_k[2][k-1]=distancia;
        matriz_k[1][k-1]=matriz[fil][tam_col-1];
        //ordenar por metodo burbuja
        for(i=0;i<k;i++){
          for(j=i+1;j<k-1;j++){
            if(matriz_k[2][i]>matriz_k[2][j]){
              //distancia
              aux=matriz_k[2][i];
              matriz_k[2][i]=matriz_k[2][j];
              matriz_k[2][j]=aux;

              //etiquetas
              aux_eti=matriz_k[1][i];
              matriz_k[1][i]=matriz_k[1][j];
              matriz_k[1][j]=aux_eti;
            }//end if
          }//end for
      }
      //fin ordenamiento 
    }
    //fin condicion de numero menor
    sumatoria=0;
  }
  
  //contar por columna, cuantos vecinos hay por etiqueta
 for(i=0;i<etiquetas;i++){ //con respecto al encerado de la primera matriz
  for (j=0;j<k;j++){ //recorrido del vector con respecto a los k vecinos 
    if(matriz_etiq[0][i]==matriz_k[1][j]){  //comparacion entre la matriz etiq almacena las etiquetas de los vecinos y la matriz k almacena todos los datos
      matriz_etiq[1][i]=1+matriz_etiq[1][i]; //suma despues de comparacion 
    }
  }
 
  
 }
 //buscar la etiqueta con mayor numero de vecinos
 for(i=0;i<etiquetas-1;i++){ 
  if(matriz_etiq[1][i]< matriz_etiq[1][i+1]){
    etiqueta=matriz_etiq[0][i+1];
  }
    else
     etiqueta=matriz_etiq[0][i];
  }
  return etiqueta;
 }
  

  
