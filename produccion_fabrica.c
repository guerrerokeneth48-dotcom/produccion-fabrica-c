#include <stdio.h>

char nombres[5][50];
float tiempos[5];
float recursos[5];
int cantidades[5];
int eliminado[5];
int total = 0;

int comparar(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 0;
        }
        i = i + 1;
    }
    if (a[i] == '\0' && b[i] == '\0') {
        return 1;
    }
    return 0;
}

int buscar(char nombre[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0 && comparar(nombres[i], nombre) == 1) {
            return i;
        }
    }
    return -1;
}

int ingresar() {
    int n, i;

    if (total >= 5) {
        printf("No hay espacio para mas productos.\n");
        return 0;
    }

    printf("Cuantos productos quiere ingresar? ");
    scanf("%d", &n);
    getchar();

    for (i = total; i < total + n; i++) {
        printf("Nombre del producto %d: ", i + 1);
        fgets(nombres[i], 50, stdin);
        int j = 0;
        while (nombres[i][j] != '\n' && nombres[i][j] != '\0') j = j + 1;
        nombres[i][j] = '\0';

        printf("Tiempo de fabricacion por unidad (horas): ");
        scanf("%f", &tiempos[i]);

        printf("Recursos por unidad: ");
        scanf("%f", &recursos[i]);

        printf("Cantidad demandada: ");
        scanf("%d", &cantidades[i]);
        getchar();

        eliminado[i] = 0;
    }

    total = total + n;
    printf("Productos ingresados.\n");
    return 0;
}

int mostrar() {
    int i;
    printf("\nLista de productos:\n");
    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0) {
            printf("Nombre: %s | Tiempo: %.2f | Recursos: %.2f | Cantidad: %d\n",
                nombres[i], tiempos[i], recursos[i], cantidades[i]);
        }
    }
    return 0;
}

int editar() {
    char nombre[50];
    int indice, opcion;

    printf("Nombre del producto a editar: ");
    fgets(nombre, 50, stdin);
    int j = 0;
    while (nombre[j] != '\n' && nombre[j] != '\0') j = j + 1;
    nombre[j] = '\0';

    indice = buscar(nombre);

    if (indice == -1) {
        printf("Producto no encontrado.\n");
        return 0;
    }

    printf("1. Nombre\n2. Tiempo\n3. Recursos\n4. Cantidad\n");
    printf("Que desea editar? ");
    scanf("%d", &opcion);
    getchar();

    if (opcion == 1) {
        printf("Nuevo nombre: ");
        fgets(nombres[indice], 50, stdin);
        int k = 0;
        while (nombres[indice][k] != '\n' && nombres[indice][k] != '\0') k = k + 1;
        nombres[indice][k] = '\0';
    } else if (opcion == 2) {
        printf("Nuevo tiempo: ");
        scanf("%f", &tiempos[indice]);
        getchar();
    } else if (opcion == 3) {
        printf("Nuevos recursos: ");
        scanf("%f", &recursos[indice]);
        getchar();
    } else if (opcion == 4) {
        printf("Nueva cantidad: ");
        scanf("%d", &cantidades[indice]);
        getchar();
    }

    printf("Producto editado.\n");
    return 0;
}

int eliminar() {
    char nombre[50];
    int indice;
    char confirmar;

    printf("Nombre del producto a eliminar: ");
    fgets(nombre, 50, stdin);
    int j = 0;
    while (nombre[j] != '\n' && nombre[j] != '\0') j = j + 1;
    nombre[j] = '\0';

    indice = buscar(nombre);

    if (indice == -1) {
        printf("Producto no encontrado.\n");
        return 0;
    }

    printf("Esta seguro? (s/n): ");
    scanf("%c", &confirmar);
    getchar();

    if (confirmar == 's') {
        eliminado[indice] = 1;
        printf("Producto eliminado.\n");
    }

    return 0;
}

int calcular() {
    int i;
    float tiempoTotal = 0;
    float recursosTotal = 0;
    float *pt = &tiempoTotal;
    float *pr = &recursosTotal;

    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0) {
            *pt = *pt + (tiempos[i] * cantidades[i]);
            *pr = *pr + (recursos[i] * cantidades[i]);
        }
    }

    printf("Tiempo total: %.2f horas\n", tiempoTotal);
    printf("Recursos totales: %.2f\n", recursosTotal);
    return 0;
}

int verificar() {
    int i;
    float tiempoTotal = 0;
    float recursosTotal = 0;
    float tiempoDisp, recursosDisp;

    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0) {
            tiempoTotal = tiempoTotal + (tiempos[i] * cantidades[i]);
            recursosTotal = recursosTotal + (recursos[i] * cantidades[i]);
        }
    }

    printf("Tiempo disponible (horas): ");
    scanf("%f", &tiempoDisp);
    printf("Recursos disponibles: ");
    scanf("%f", &recursosDisp);
    getchar();

    if (tiempoTotal <= tiempoDisp && recursosTotal <= recursosDisp) {
        printf("La fabrica SI puede cumplir con la demanda.\n");
    } else {
        printf("La fabrica NO puede cumplir con la demanda.\n");
        if (tiempoTotal > tiempoDisp) {
            printf("Le faltan %.2f horas.\n", tiempoTotal - tiempoDisp);
        }
        if (recursosTotal > recursosDisp) {
            printf("Le faltan %.2f recursos.\n", recursosTotal - recursosDisp);
        }
    }

    return 0;
}

int main() {
    int opcion;
    int i;

    for (i = 0; i < 5; i++) {
        eliminado[i] = 0;
    }

    do {
        printf("\n1. Ingresar productos\n");
        printf("2. Mostrar productos\n");
        printf("3. Editar producto\n");
        printf("4. Eliminar producto\n");
        printf("5. Calcular produccion\n");
        printf("6. Verificar capacidad\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        if (opcion == 1) ingresar();
        else if (opcion == 2) mostrar();
        else if (opcion == 3) editar();
        else if (opcion == 4) eliminar();
        else if (opcion == 5) calcular();
        else if (opcion == 6) verificar();

    } while (opcion != 0);

    return 0;
}
