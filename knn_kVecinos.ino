#include "matriz_entrenamiento.h"
//float datos_prueba[5] = {4.8, 3, 1.4, 0.3, 1}; // eti 1
float datos_prueba[5] = {5.1, 2.5, 3, 1.1, 2}; // eti 2
//float datos_prueba[5] = {6.7, 3, 5.2, 2.3, 3}; //eti 3
int respuesta;
void setup() {
  Serial.begin(9600);
}

void loop() {
  respuesta = knn(3, 3, 120, 5);
  delay(5000);
}

int knn(int k, int etiquetas, int tam_fil, int tam_col) {
  int i = 0;
  int j = 0;
  int col;
  int fil;
  float eti;
  float aux_dis;
  float aux_eti;
  float sumatoria = 0; //sumatoria de la elevacion al cuadrado de cada columna
  float distancia = 0; //raiz cuadrada de sumatoria
  /*
     matriz de k=vecinos
     |1|2|3| --> define los vecinos
     |2|1|1| --> aignacion del vecino por etiqueta
     |0.1|0.2|0.3| --> distancia
  */
  float matriz_k[3][k];
  /*
     etiquetas
     |1|2|3| --> etiquetas que exiten en la base de datos
     |2|1|0| --> conteo de etiquetas dentro de los k=vecinos
  */
  int matriz_etique[2][etiquetas];
  //asignar numero de vecinos y enceramos asignacion de etiquetas
  for (; i < k; i++) {
    matriz_k[0][i] = i + 1;
    matriz_k[1][i] = 0;
    matriz_k[2][i] = 3000.0 + i;
    //Serial.println(matriz_k[2][i]);
    /*
       |1|2|3|
       |0|0|0|
       |3000|3001|3002|
    */
  }
  //asignar etiquetas y enceramos el contador
  for (int i = 0; i < etiquetas; i++) {
    matriz_etique[0][i] = i + 1;
    matriz_etique[1][i] = 0;
    /*
      |1|2|3|
      |0|0|0|
    */
  }
  //pasos para knn
  /*
     leer cada fila de la matriz de entrenamiento
     sacar la distancia entre fila de matriz con nueva instancia (vector de prueba)
     ordenar la matriz -> matriz_k (determinar los k vecinos de menor distancia)
     contar las etiquetas en la matriz_k y asignar en matriz_etiquetas
     elegir el mayor numero de matriz_etiquetas
     retorno etiqueta resultante
  */

  for (fil = 0; fil < tam_fil; fil++) {
    for (col = 0; col < tam_col - 1; col++) { //ultima columna es etiqueta
      sumatoria = sumatoria + pow(matriz[fil][col] - datos_prueba[col], 2);
    }
    distancia = sqrt(sumatoria);
    //comparacion de nuevo dato con solo la distancia mayor almacenada en matriz_k
    if (distancia < matriz_k[2][k - 1]) {
      matriz_k[2][k - 1] = distancia;
      matriz_k[1][k - 1] = matriz[fil][tam_col - 1];
      //ordenarmiento

      for (i = 0; i < k; i++) {
        for (j = i + 1; j < k; j++) {
          if (matriz_k[2][i] > matriz_k[2][j]) {
            //distancia
            aux_dis = matriz_k[2][j];
            matriz_k[2][j] = matriz_k[2][i];
            matriz_k[2][i] = aux_dis;
            //etiqueta
            aux_eti = matriz_k[1][j];
            matriz_k[1][j] = matriz_k[1][i];
            matriz_k[1][i] = aux_eti;

          }//end if
        }//end for
      }
      //fin ordenamiento
    }
    //fin condicion numero menor
    sumatoria = 0;
  }//fin de lectura de matriz

  //*******************************************************************
  // SE CUENTA POR CADA COLUMNA EL NUMERO DE VECINOS QUE EXISTEN
  for (i = 0; i < k; i++) {
    if (matriz_k[1][i] == 1.0 ) {//          secondiciona si para la primera posicion es de etiqueta 1, a la celda de matriz_etique se suma su valor
      matriz_etique[1][0] ++ ;      //       y se repite el mismo porceso para las etiquetas de 2 y de 3, y se suman los valores para cada celda de matriz_etique
    } else if (matriz_k[1][i] == 2.0 ) { //  segun la etiqueta correcta respectivamente.
      matriz_etique[1][1] ++ ;
    } else if (matriz_k[1][i] == 3.0 ) {
      matriz_etique[1][2] ++ ;
    }
  }

  //*******************************************************************
  // AQUI SE BUSCA EL MAYOR VALOR DE ETIUQETAS
  for (i = 0; i < etiquetas - 1; i++) {
    if (matriz_etique[1][i] > matriz_etique[1][i + 1]) {//en este caso solo evalua si es de etiqueta 1 o 2
      eti = matriz_etique[0][i];
    } else if (matriz_etique[1][i] < matriz_etique[1][i + 1]) {// y aqui evalua si es de etiqueta 3
      eti = matriz_etique[0][i + 1];// para este caso se suma 1 para dar el valor de 3
    }
    else
      eti = matriz_etique[0][i - 1];// y aqui se resta 1 para dar el valor minimo de 1 o maximo de 2
  }

  Serial.println("MATRIZ DE VECINOS");
  for (i = 0; i < 3; i++) {
    for (j = 0; j < k; j++) {
      Serial.print(matriz_k[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  Serial.println(" ");
  Serial.println("MATRIZ DE ETIQUETAS");
  for (i = 0; i < 2; i++) {
    for (j = 0; j < etiquetas; j++) {
      Serial.print(matriz_etique[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  Serial.println(" ");
  if (eti == datos_prueba[tam_col - 1]) {
    Serial.print("la etiqueta es: ");
    Serial.print(eti);
  } else {
    Serial.print("falso");
  }
}
