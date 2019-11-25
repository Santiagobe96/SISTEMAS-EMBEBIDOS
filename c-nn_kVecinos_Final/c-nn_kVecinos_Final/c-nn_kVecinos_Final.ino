/*
                    UNIVERSIDAD TECNICA DEL NORTE
                              CIERCOM
   Nombre: Bolaños Santiago
   Asignatura: Sistemas Embebidos
   Actividad: Deber 03_Primer Bimestre
   Tema: c-nn_Acabado
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
  delay(300);
}

char cnn(int k, int etiquetas, int tam_fil, int tam_col) {
  int i, j, m, l; //variables de for
  int col;
  int fil;
  int cont = 0; //variable contador
  float sumatoria;
  float distancia;
  float centroid = 0;

  //float centroides[120][5];
  //lo que se hace es dimensioanr la matriz de centroides al limite de la mztriz
  //de entrenamiento, y como al momento de llenar los centroides se limita a etiqueta=3
  //no habria ningun problema, esto se hace para poder dimensioanr la misma matriz
  //y poder usarla en matriz de source, ya que en extremo caso si mas de la mitad de
  //de muestras estan en source, si hay espacio en matriz centroides para llenar los datos
  float centroides[tam_fil][tam_col]; //matriz de centroides
  for (i = 0; i < tam_fil; i++) {
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
  //Serial.println(" ");
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
    centroides[l][tam_col - 1] = l + 1;
    Serial.println(centroides[l][tam_col - 1]);
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
          for (int s = 0 ; s < tam_col; s++) {
            centroides[cont + etiquetas][s] = matriz[i][s];
          }
          cont++; //ademas como el algoritmo es c-nn se debe contar los valores de source
        }
      }
      Serial.print(matriz[i][j]); //imprimir datos de matriz
      Serial.print(' ');
    }
  }
  Serial.println("");
  Serial.print("LA CANTIDAD DE MUESTRAS EN SOURCE ES: ");
  Serial.println(cont + etiquetas); //imprimir resultado
  Serial.println("MATRIZ DE SOURCE");
  for (int sf = 0; sf < cont + etiquetas; sf++) {
    for (int sc = 0; sc < tam_col; sc++) {
      Serial.print(centroides[sf][sc]); //imprimir datos de matriz
      Serial.print(' ');
    }
    Serial.println(' ');
  }
  Serial.println(" ");
}
