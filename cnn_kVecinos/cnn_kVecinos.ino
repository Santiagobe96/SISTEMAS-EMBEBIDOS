/*
                    UNIVERSIDAD TECNICA DEL NORTE
                              CIERCOM
   Nombre: Bolaños Santiago
   Asignatura: Sistemas Embebidos
   Actividad: Deber 02_Primer Bimestre
   Tema: c-nn
*/
#include "matriz_entrenamiento.h"
float datos_prueba[5] = {5.9, 3, 5.1, 1.8, 3};
int respuesta;
char resp_cnn; //respuesta de algoritmo cnn
void setup() {
  Serial.begin(9600);
}

void loop() {
  resp_cnn = cnn(3, 3, 120, 5); //Llamado del método
  //  respuesta = knn(3, 3, 120, 5);
  //  Serial.print(respuesta);
  delay(30000);
}

char cnn(int k, int etiquetas, int tam_fil, int tam_col) {
  int i, j, m, l; //variables de for
  int col;
  int fil;
  int cont = 0; //variable contador
  float sumatoria;
  float distancia;
  float centroid = 0;
  float centroides[etiquetas][tam_col]; //matriz de centroides
  for (i = 0; i < etiquetas; i++) {
    for (j = 0; j < tam_col; j++) {
      centroides[i][j] = 0; //encerar matriz
    }
  }
  float distancias[tam_fil][etiquetas]; //matriz de distancias
  for (i = 0; i < tam_fil; i++) {
    for (j = 0; j < etiquetas; j++) {
      distancias[i][j] = 0; //encerar matriz
    }
  }
  int vec_eti[tam_fil]; //vector de las etiquetas halladas
  for (j = 0; j < tam_fil; j++) {
    vec_eti[j] = 0; //encerar vector
  }

  Serial.println("MATRIZ DE LOS CENTROIDES");
  Serial.println("");
  for (l = 0; l < etiquetas; l++) { //lee filas
    for (i = 0; i < tam_col - 1; i++) { //lee columnas
      for (j = (40 * l); j < 40 * (l + 1); j++) { //limita a que se lea cada 40 datos
        centroid = centroid + matriz[j][i]; //suma los datos de una columna de la matriz de entrenamiento
      }
      centroides[l][i] = centroid / 40; //promedia la suma de los datos de cada columande la matria de entrenamiento
      Serial.print(centroides[l][i]);
      Serial.print(',');
      centroid = 0; //volver a cero el valor de centroid para realizar la suma de la siguiente columna
    }
    Serial.println(' ');
  }

  for (l = 0; l < etiquetas; l++) { //lee filas de matriz centroides
    for (fil = 0; fil < tam_fil; fil++) { //lee filas de matriz de entrenamiento y matriz centroides
      for (col = 0; col < tam_col - 1; col++) { //lee columnas de matriz de entrenamiento
        sumatoria = sumatoria + pow(matriz[fil][col] - centroides[l][col], 2); //suma las potencnias de los dos puntos a calcular la distancia
      }
      distancia = sqrt(sumatoria); //raiz cuadrada de la suma de los dis puntos elevados al cuadrado
      distancias[fil][l] = distancia; //llenar matriz de distancias
      sumatoria = 0; //encerar variable
    }
  }
  for (i = 0; i < tam_fil; i++) { //lee filas
    for (j = 0; j < etiquetas; j++) { //lee columna uno
      for (m = j + 1; m < etiquetas; m++) { //lee columna dos
        for (l = m + 1; l < etiquetas; l++) { //lee columna tres
          //************* esto se hace con el objetivo de recorrer en una misma fila, las distintas columnas de la matriz de distancias******************
          if (distancias[i][j] < distancias[i][m] && distancias[i][j] < distancias[i][l]) { //compara datos en las distintas posiciones
            vec_eti[i] = 1; //si dato en posición 0 es menor, vec_eti=1
          } else if (distancias[i][m] < distancias[i][j] && distancias[i][m] < distancias[i][l]) {
            vec_eti[i] = 2; //si dato en posición 1 es menor, vec_eti=2
          } else if (distancias[i][l] < distancias[i][m] && distancias[i][l] < distancias[i][j]) {
            vec_eti[i] = 3; //si dato en posición 2 es menor, vec_eti=3
          }
        }
      }
    }
  }

  for (i = 0; i < tam_fil; i++) { //lee filas
    for (j = 1; j < tam_col + 1; j++) { //lee columnas
      if (j == tam_col ) {
        //************* esta es la seccion de validacion para solo imprimir en la ultima posicion y compararla entre etiqueta
        //************* hallada de distancias menores y etiqueta ya asiganda en matriz de entrenamiento

        if (vec_eti[i] == matriz[i][tam_col - 1]) { //comparar la etiqueta de entrenamiento en ultima posicion vs valor de vector etiqueta
          Serial.println('D'); //si son iguales se van al drenaje
        } else {
          Serial.println('S'); //si son distintas se van al source
          cont++; //ademas como el algoritmo es c-nn se debe contar los valores de source
        }
      }
      Serial.print(matriz[i][j]); //imprimir datos de matriz
      Serial.print(' ');
    }
  }
  Serial.println("");
  Serial.print("LA CANTIDAD DE MUESTRAS EN SOURCE ES: ");
  Serial.println(cont); //imprimir resultado
}
//************************METODO DE k-nn  YA REALIZADO****************************
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
}
