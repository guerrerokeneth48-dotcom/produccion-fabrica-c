/*
 * Sistema de Optimizacion de Produccion - Fabrica de Componentes Electronicos
 * ISWZ1102 - Programacion I
 * Implementado con arreglos paralelos y punteros (sin structs ni archivos)
 */

#include <stdio.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE    50

/* ── Arreglos paralelos globales ── */
char  nombres[MAX_PRODUCTOS][MAX_NOMBRE];
float tiempoPorUnidad[MAX_PRODUCTOS];   /* horas de fabricacion por unidad */
float recursosPorUnidad[MAX_PRODUCTOS]; /* unidades de recurso por producto */
int   cantidadDemandada[MAX_PRODUCTOS]; /* unidades demandadas              */
int   activo[MAX_PRODUCTOS];            /* 1 = activo, 0 = eliminado        */
int   numProductos;                     /* cantidad de slots ocupados        */

/* ──────────────────────────────────────────────────────────── */
/*  UTILIDADES                                                  */
/* ──────────────────────────────────────────────────────────── */

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Copia src → dst hasta MAX_NOMBRE-1 caracteres (sin strcpy) */
void copiarCadena(char *dst, char *src, int maxLen) {
    int i = 0;
    while (i < maxLen - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* Lee una linea de stdin, elimina el '\n' final */
void leerLinea(char *buffer, int maxLen) {
    int i = 0;
    char c;
    while (i < maxLen - 1 && (c = getchar()) != '\n' && c != EOF) {
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

/* Convierte un caracter a minuscula */
char aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

/* ──────────────────────────────────────────────────────────── */
/*  BUSQUEDA PROPIA (sin funciones predefinidas)                */
/* ──────────────────────────────────────────────────────────── */

/*
 * Busqueda lineal insensible a mayusculas/minusculas.
 * Retorna el indice del producto o -1 si no se encuentra.
 */
int buscarPorNombre(char *nombreBuscar) {
    int i, j, igual;
    for (i = 0; i < numProductos; i++) {
        if (!activo[i]) continue;
        igual = 1;
        for (j = 0; ; j++) {
            char a = aMinuscula(nombreBuscar[j]);
            char b = aMinuscula(nombres[i][j]);
            if (a != b) { igual = 0; break; }
            if (a == '\0') break; /* ambos terminaron iguales */
        }
        if (igual) return i;
    }
    return -1;
}

/* ──────────────────────────────────────────────────────────── */
/*  CONTAR PRODUCTOS ACTIVOS                                    */
/* ──────────────────────────────────────────────────────────── */

int contarActivos(void) {
    int i, cont = 0;
    for (i = 0; i < numProductos; i++)
        if (activo[i]) cont++;
    return cont;
}

/* ──────────────────────────────────────────────────────────── */
/*  MODULO 1 – INGRESO DE PRODUCTOS                            */
/* ──────────────────────────────────────────────────────────── */

void ingresarProductos(void) {
    int i, cantidad, inicio;

    printf("\n=== INGRESO DE PRODUCTOS ===\n");

    if (numProductos >= MAX_PRODUCTOS) {
        printf("Ya se alcanzó el maximo de %d productos.\n", MAX_PRODUCTOS);
        return;
    }

    int disponibles = MAX_PRODUCTOS - contarActivos();
    printf("Slots disponibles: %d\n", disponibles);
    printf("¿Cuantos productos desea ingresar? ");
    scanf("%d", &cantidad);
    limpiarBuffer();

    if (cantidad <= 0) { printf("Cantidad invalida.\n"); return; }
    if (cantidad > disponibles) {
        cantidad = disponibles;
        printf("Se ajusto a %d producto(s).\n", cantidad);
    }

    inicio = numProductos;
    for (i = inicio; i < inicio + cantidad; i++) {
        printf("\n--- Producto %d de %d ---\n", i - inicio + 1, cantidad);

        printf("Nombre del producto          : ");
        leerLinea(nombres[i], MAX_NOMBRE);

        printf("Tiempo de fabricacion (h/u)  : ");
        scanf("%f", &tiempoPorUnidad[i]);

        printf("Recursos por unidad          : ");
        scanf("%f", &recursosPorUnidad[i]);

        printf("Cantidad demandada (unidades): ");
        scanf("%d", &cantidadDemandada[i]);
        limpiarBuffer();

        activo[i] = 1;
        numProductos++;
    }
    printf("\n✓ %d producto(s) ingresado(s) exitosamente.\n", cantidad);
}

/* ──────────────────────────────────────────────────────────── */
/*  MODULO 2 – MOSTRAR PRODUCTOS                               */
/* ──────────────────────────────────────────────────────────── */

void mostrarProductos(void) {
    int i, n = 1;
    printf("\n=== LISTA DE PRODUCTOS ACTIVOS ===\n");
    printf("%-4s %-22s %12s %12s %10s\n",
           "N.", "Nombre", "Tiempo(h/u)", "Recursos/u", "Demanda");
    printf("--------------------------------------------------------------\n");

    for (i = 0; i < numProductos; i++) {
        if (!activo[i]) continue;
        printf("%-4d %-22s %12.2f %12.2f %10d\n",
               n++, nombres[i], tiempoPorUnidad[i],
               recursosPorUnidad[i], cantidadDemandada[i]);
    }
    if (n == 1) printf("No hay productos registrados.\n");
}

/* ──────────────────────────────────────────────────────────── */
/*  MODULO 3 – EDICION DE PRODUCTO                             */
/* ──────────────────────────────────────────────────────────── */

void editarProducto(void) {
    char nombreBuscar[MAX_NOMBRE];
    int opcion, indice;

    printf("\n=== EDICION DE PRODUCTO ===\n");
    printf("Nombre del producto a editar: ");
    leerLinea(nombreBuscar, MAX_NOMBRE);

    indice = buscarPorNombre(nombreBuscar);
    if (indice == -1) {
        printf("Producto '%s' no encontrado.\n", nombreBuscar);
        return;
    }

    printf("Producto encontrado: %s\n", nombres[indice]);
    printf("\n¿Que desea editar?\n");
    printf("  1. Nombre\n");
    printf("  2. Tiempo de fabricacion por unidad\n");
    printf("  3. Recursos por unidad\n");
    printf("  4. Cantidad demandada\n");
    printf("  5. Todos los campos\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    limpiarBuffer();

    switch (opcion) {
        case 1:
            printf("Nuevo nombre: ");
            leerLinea(nombres[indice], MAX_NOMBRE);
            break;
        case 2:
            printf("Nuevo tiempo de fabricacion (h/u): ");
            scanf("%f", &tiempoPorUnidad[indice]);
            limpiarBuffer();
            break;
        case 3:
            printf("Nuevos recursos por unidad: ");
            scanf("%f", &recursosPorUnidad[indice]);
            limpiarBuffer();
            break;
        case 4:
            printf("Nueva cantidad demandada: ");
            scanf("%d", &cantidadDemandada[indice]);
            limpiarBuffer();
            break;
        case 5:
            printf("Nuevo nombre: ");
            leerLinea(nombres[indice], MAX_NOMBRE);
            printf("Nuevo tiempo de fabricacion (h/u): ");
            scanf("%f", &tiempoPorUnidad[indice]);
            printf("Nuevos recursos por unidad: ");
            scanf("%f", &recursosPorUnidad[indice]);
            printf("Nueva cantidad demandada: ");
            scanf("%d", &cantidadDemandada[indice]);
            limpiarBuffer();
            break;
        default:
            printf("Opcion invalida.\n");
            return;
    }
    printf("✓ Producto editado exitosamente.\n");
}

/* ──────────────────────────────────────────────────────────── */
/*  MODULO 4 – ELIMINACION DE PRODUCTO                         */
/* ──────────────────────────────────────────────────────────── */

void eliminarProducto(void) {
    char nombreBuscar[MAX_NOMBRE];
    char confirmacion[4];
    int indice;

    printf("\n=== ELIMINACION DE PRODUCTO ===\n");
    printf("Nombre del producto a eliminar: ");
    leerLinea(nombreBuscar, MAX_NOMBRE);

    indice = buscarPorNombre(nombreBuscar);
    if (indice == -1) {
        printf("Producto '%s' no encontrado.\n", nombreBuscar);
        return;
    }

    printf("Producto encontrado: %s\n", nombres[indice]);
    printf("¿Confirma la eliminacion? (s/n): ");
    leerLinea(confirmacion, 4);

    if (confirmacion[0] == 's' || confirmacion[0] == 'S') {
        activo[indice] = 0;
        printf("✓ Producto eliminado exitosamente.\n");
    } else {
        printf("Eliminacion cancelada.\n");
    }
}

/* ──────────────────────────────────────────────────────────── */
/*  MODULO 5 – CALCULO DE PRODUCCION                           */
/* ──────────────────────────────────────────────────────────── */

void calcularProduccion(void) {
    int i;
    float tiempoTotal = 0.0f, recursosTotal = 0.0f;
    float *pTiempo    = tiempoPorUnidad;    /* uso de punteros */
    float *pRecursos  = recursosPorUnidad;
    int   *pDemanda   = cantidadDemandada;

    printf("\n=== CALCULO DE PRODUCCION ===\n");

    if (contarActivos() == 0) {
        printf("No hay productos para calcular.\n");
        return;
    }

    printf("%-22s %12s %12s %12s\n",
           "Producto", "Demanda", "T.Total(h)", "R.Total");
    printf("--------------------------------------------------------------\n");

    for (i = 0; i < numProductos; i++) {
        if (!activo[i]) continue;
        float tProd = *(pTiempo + i)   * *(pDemanda + i);
        float rProd = *(pRecursos + i) * *(pDemanda + i);
        tiempoTotal    += tProd;
        recursosTotal  += rProd;
        printf("%-22s %12d %12.2f %12.2f\n",
               nombres[i], cantidadDemandada[i], tProd, rProd);
    }

    printf("--------------------------------------------------------------\n");
    printf("%-22s %12s %12.2f %12.2f\n",
           "TOTAL", "", tiempoTotal, recursosTotal);
    printf("\nTiempo total de fabricacion : %.2f horas\n", tiempoTotal);
    printf("Recursos totales necesarios : %.2f unidades\n", recursosTotal);
}

/* ──────────────────────────────────────────────────────────── */
/*  MODULO 6 – VERIFICACION DE CAPACIDAD                       */
/* ──────────────────────────────────────────────────────────── */

void verificarCapacidad(void) {
    int i;
    float tiempoTotal = 0.0f, recursosTotal = 0.0f;
    float tiempoDisp, recursosDisp;

    printf("\n=== VERIFICACION DE CAPACIDAD ===\n");

    if (contarActivos() == 0) {
        printf("No hay productos para verificar.\n");
        return;
    }

    for (i = 0; i < numProductos; i++) {
        if (!activo[i]) continue;
        tiempoTotal   += tiempoPorUnidad[i]   * cantidadDemandada[i];
        recursosTotal += recursosPorUnidad[i] * cantidadDemandada[i];
    }

    printf("Tiempo total requerido   : %.2f horas\n",   tiempoTotal);
    printf("Recursos totales requeridos: %.2f unidades\n", recursosTotal);

    printf("\nIngrese el tiempo disponible (horas): ");
    scanf("%f", &tiempoDisp);
    printf("Ingrese los recursos disponibles    : ");
    scanf("%f", &recursosDisp);
    limpiarBuffer();

    printf("\n--- RESULTADO DE CAPACIDAD ---\n");

    int puedeT = (tiempoTotal    <= tiempoDisp);
    int puedeR = (recursosTotal  <= recursosDisp);

    if (puedeT && puedeR) {
        printf("✓ La fabrica PUEDE cumplir con la demanda.\n");
        printf("  Tiempo sobrante   : %.2f horas\n",    tiempoDisp   - tiempoTotal);
        printf("  Recursos sobrantes: %.2f unidades\n", recursosDisp - recursosTotal);
    } else {
        printf("✗ La fabrica NO PUEDE cumplir con la demanda.\n");
        if (!puedeT)
            printf("  Deficit de tiempo   : %.2f horas\n",    tiempoTotal    - tiempoDisp);
        if (!puedeR)
            printf("  Deficit de recursos : %.2f unidades\n", recursosTotal  - recursosDisp);
    }
}

/* ──────────────────────────────────────────────────────────── */
/*  MENU PRINCIPAL                                              */
/* ──────────────────────────────────────────────────────────── */

void mostrarMenu(void) {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║  SISTEMA DE OPTIMIZACION DE PRODUCCION  ║\n");
    printf("║       Fabrica de Componentes Electronicos║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  1. Ingresar productos                   ║\n");
    printf("║  2. Mostrar productos                    ║\n");
    printf("║  3. Editar producto                      ║\n");
    printf("║  4. Eliminar producto                    ║\n");
    printf("║  5. Calcular produccion                  ║\n");
    printf("║  6. Verificar capacidad de la fabrica    ║\n");
    printf("║  0. Salir                                ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("Seleccione una opcion: ");
}

/* ──────────────────────────────────────────────────────────── */
/*  FUNCION PRINCIPAL                                           */
/* ──────────────────────────────────────────────────────────── */

int main(void) {
    int opcion, i;

    /* Inicializar arreglos */
    numProductos = 0;
    for (i = 0; i < MAX_PRODUCTOS; i++) {
        activo[i]            = 0;
        nombres[i][0]        = '\0';
        tiempoPorUnidad[i]   = 0.0f;
        recursosPorUnidad[i] = 0.0f;
        cantidadDemandada[i] = 0;
    }

    printf("╔══════════════════════════════════════════╗\n");
    printf("║   Bienvenido al Sistema de Produccion    ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            printf("Entrada invalida. Ingrese un numero.\n");
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1: ingresarProductos();   break;
            case 2: mostrarProductos();    break;
            case 3: editarProducto();      break;
            case 4: eliminarProducto();    break;
            case 5: calcularProduccion();  break;
            case 6: verificarCapacidad();  break;
            case 0: printf("Saliendo... hasta pronto.\n"); break;
            default: printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 0);

    return 0;
}
