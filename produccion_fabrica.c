#include <stdio.h>
#include <string.h>

#define MAX 5

/* Arreglos para guardar los datos de los productos */
char nombres[MAX][50];
float tiempos[MAX];
float recursos[MAX];
int cantidades[MAX];
int eliminado[MAX];
int total = 0;

/* Busqueda propia por nombre - sin funciones predefinidas de busqueda */
int buscar(char nombre[]) {
    int i;
    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0 && strcmp(nombres[i], nombre) == 0) {
            return i;
        }
    }
    return -1;
}

void ingresar() {
    int n, i;

    if (total >= MAX) {
        printf("Ya no hay espacio para mas productos.\n");
        return;
    }

    printf("Cuantos productos quiere ingresar? ");
    scanf("%d", &n);
    getchar();

    if (total + n > MAX) {
        n = MAX - total;
        printf("Solo puede ingresar %d producto(s) mas.\n", n);
    }

    for (i = total; i < total + n; i++) {
        printf("\nProducto %d:\n", i + 1);

        printf("Nombre: ");
        fgets(nombres[i], 50, stdin);
        nombres[i][strlen(nombres[i]) - 1] = '\0';

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
    printf("\nProductos ingresados correctamente.\n");
}

void mostrar() {
    int i, hay = 0;

    printf("\n--- Lista de productos ---\n");
    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0) {
            printf("Nombre: %s\n", nombres[i]);
            printf("  Tiempo: %.2f h/u | Recursos: %.2f/u | Cantidad: %d\n",
                   tiempos[i], recursos[i], cantidades[i]);
            hay = 1;
        }
    }

    if (hay == 0) {
        printf("No hay productos registrados.\n");
    }
}

void editar() {
    char nombre[50];
    int indice, opcion;

    printf("Nombre del producto a editar: ");
    fgets(nombre, 50, stdin);
    nombre[strlen(nombre) - 1] = '\0';

    indice = buscar(nombre);

    if (indice == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    printf("Producto encontrado: %s\n", nombres[indice]);
    printf("Que desea editar?\n");
    printf("1. Nombre\n");
    printf("2. Tiempo de fabricacion\n");
    printf("3. Recursos por unidad\n");
    printf("4. Cantidad demandada\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar();

    if (opcion == 1) {
        printf("Nuevo nombre: ");
        fgets(nombres[indice], 50, stdin);
        nombres[indice][strlen(nombres[indice]) - 1] = '\0';
    } else if (opcion == 2) {
        printf("Nuevo tiempo (h/u): ");
        scanf("%f", &tiempos[indice]);
        getchar();
    } else if (opcion == 3) {
        printf("Nuevos recursos por unidad: ");
        scanf("%f", &recursos[indice]);
        getchar();
    } else if (opcion == 4) {
        printf("Nueva cantidad demandada: ");
        scanf("%d", &cantidades[indice]);
        getchar();
    } else {
        printf("Opcion no valida.\n");
        return;
    }

    printf("Producto editado correctamente.\n");
}

void eliminar() {
    char nombre[50];
    int indice;
    char confirmar;

    printf("Nombre del producto a eliminar: ");
    fgets(nombre, 50, stdin);
    nombre[strlen(nombre) - 1] = '\0';

    indice = buscar(nombre);

    if (indice == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    printf("Producto encontrado: %s\n", nombres[indice]);
    printf("Esta seguro? (s/n): ");
    scanf("%c", &confirmar);
    getchar();

    if (confirmar == 's' || confirmar == 'S') {
        eliminado[indice] = 1;
        printf("Producto eliminado.\n");
    } else {
        printf("Eliminacion cancelada.\n");
    }
}

void calcular() {
    int i;
    float tiempoTotal = 0;
    float recursosTotal = 0;
    float *pt = tiempos;   /* uso de punteros */
    float *pr = recursos;

    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0) {
            tiempoTotal   = tiempoTotal   + (*(pt + i) * cantidades[i]);
            recursosTotal = recursosTotal + (*(pr + i) * cantidades[i]);
        }
    }

    printf("\n--- Resultado del calculo ---\n");
    printf("Tiempo total de fabricacion : %.2f horas\n", tiempoTotal);
    printf("Recursos totales necesarios : %.2f unidades\n", recursosTotal);
}

void verificar() {
    int i;
    float tiempoTotal = 0;
    float recursosTotal = 0;
    float tiempoDisp, recursosDisp;

    for (i = 0; i < total; i++) {
        if (eliminado[i] == 0) {
            tiempoTotal   = tiempoTotal   + (tiempos[i]   * cantidades[i]);
            recursosTotal = recursosTotal + (recursos[i]  * cantidades[i]);
        }
    }

    printf("Tiempo requerido  : %.2f horas\n",   tiempoTotal);
    printf("Recursos requeridos: %.2f unidades\n", recursosTotal);

    printf("\nCuanto tiempo tiene disponible la fabrica (horas)? ");
    scanf("%f", &tiempoDisp);
    printf("Cuantos recursos tiene disponibles? ");
    scanf("%f", &recursosDisp);
    getchar();

    printf("\n--- Resultado ---\n");
    if (tiempoTotal <= tiempoDisp && recursosTotal <= recursosDisp) {
        printf("La fabrica SI puede cumplir con la demanda.\n");
        printf("Tiempo sobrante  : %.2f horas\n",    tiempoDisp   - tiempoTotal);
        printf("Recursos sobrantes: %.2f unidades\n", recursosDisp - recursosTotal);
    } else {
        printf("La fabrica NO puede cumplir con la demanda.\n");
        if (tiempoTotal > tiempoDisp) {
            printf("Le faltan %.2f horas.\n", tiempoTotal - tiempoDisp);
        }
        if (recursosTotal > recursosDisp) {
            printf("Le faltan %.2f recursos.\n", recursosTotal - recursosDisp);
        }
    }
}

int main() {
    int opcion;
    int i;

    for (i = 0; i < MAX; i++) {
        eliminado[i] = 0;
    }

    printf("=== Sistema de Produccion - Fabrica de Componentes ===\n");

    do {
        printf("\n--- MENU ---\n");
        printf("1. Ingresar productos\n");
        printf("2. Mostrar productos\n");
        printf("3. Editar producto\n");
        printf("4. Eliminar producto\n");
        printf("5. Calcular produccion\n");
        printf("6. Verificar capacidad\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        if      (opcion == 1) ingresar();
        else if (opcion == 2) mostrar();
        else if (opcion == 3) editar();
        else if (opcion == 4) eliminar();
        else if (opcion == 5) calcular();
        else if (opcion == 6) verificar();
        else if (opcion != 0) printf("Opcion no valida.\n");

    } while (opcion != 0);

    printf("Hasta luego!\n");
    return 0;
}
