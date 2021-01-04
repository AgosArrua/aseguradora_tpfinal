#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Datos.h"
#include "FuncionesCarga.h"

//como esta funcion se invoca ni bien se abre el archivo para el resto de funciones cada vez que se abra un archivo no se verificara
//que la apertura sea correcta, ya que el archivo existe, y no hay conflictos en la lectura que necesita que los archivos existan
void VerificarArchivos(){ //chequea si un archivo existe, de no existir lo crea. informa por pantalla que sucede
    FILE *Archivo;

    Archivo = fopen("archivos/Vendedores.dat", "rb"); //intenta abrir para la lectura
    if (!Archivo){ //el archivo no existe
        Archivo = fopen("archivos/Vendedores.dat", "wb"); //abre para escribir, entonces crea el archivo
        printf("\nArchivo 'Vendedores' creado.");
    } else { //el archivo existe, se pudo abrir correctamente
        printf("\nArchivo 'Vendedores' ya existe.");
    }
    fclose(Archivo);

    Archivo = fopen("archivos/Polizas.dat", "rb");
    if (!Archivo){
        Archivo = fopen("archivos/Polizas.dat", "wb");
        printf("\nArchivo 'Polizas' creado.");
    } else {
        printf("\nArchivo 'Polizas' ya existe.");
    }
    fclose(Archivo);

    Archivo = fopen("archivos/Siniestros.dat", "rb");
    if (!Archivo){
        Archivo = fopen("archivos/Siniestros.dat", "wb");
        printf("\nArchivo 'Siniestros' creado.");
    } else {
        printf("\nArchivo 'Siniestros' ya existe.");
    }
    fclose(Archivo);

    Archivo = fopen("archivos/Asegurados.dat", "rb");
    if (!Archivo){
        Archivo = fopen("archivos/Asegurados.dat", "wb");
        printf("\nArchivo 'Asegurados' creado.");
    } else {
        printf("\nArchivo 'Asegurados' ya existe.");
    }
    fclose(Archivo);
}

void Menu(){ //menu de inicio
    printf("\n\nMENU"); //menu de inicio
    printf("\n   1. Cargar datos");
    printf("\n   2. Informes");
    printf("\n   0. Salir");
}

void MenuCarga(int *p){ //muestra posibles cargas y valida que se seleccione una opcion correcta
    printf("\nCARGA:");
    printf("\n   1. Vendedor");
    printf("\n   2. Poliza");
    printf("\n   3. Siniestro");
    printf("\nOpcion: ");
    scanf("%d", p);
    while ((*p < 1) || (*p >3)){
        printf("Opcion invalida, reingrese: ");
        scanf("%d", p);
    }
}

int ValidarVendedor(palabra apellido){ //valida el apellido ingresado en carga de poliza para que solo se pueda
    int aux = -1;                       //cargar si el vendedor existe, retorna su ID si apellido esta en Vendedores.dat
    Vendedor auxV;                        //si no esta retorna -1
    FILE *ArchiVendedores;

    ArchiVendedores = fopen("archivos/Vendedores.dat", "rb");

    fread(&auxV, sizeof(Vendedor), 1, ArchiVendedores);

    while (!feof(ArchiVendedores)){
        if (strcmp(auxV.ApellidoVendedor,apellido) == 0){
            aux = auxV.IDVendedor;
        }
        fread(&auxV, sizeof(Vendedor), 1, ArchiVendedores);
    }

    fclose(ArchiVendedores);
    return aux;
}

int ValidarPoliza(int NroPoliza){ //valida el numero de poliza ingresado en la carga de siniestros para que solo se pueda
    int aux = -1;                   //cargar si esa poliza existe. Retorna 1 si la encuentra en Polizas.dat, de lo contrario
    Poliza auxP;                        //retorna -1
    FILE *ArchiPolizas;

    ArchiPolizas = fopen("archivos/Polizas.dat", "rb");

    fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);

    while (!feof(ArchiPolizas)){
        if (auxP.NroPoliza == NroPoliza){
            aux = 1;
        }
        fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);
    }

    fclose(ArchiPolizas);
    return aux;
}

void CargaSiniestro(FILE *Archivo){ //agrega un nuevo registro a Sinistros.dat
    Siniestro s;
    int auxPoliza,valido;
    Archivo = fopen("archivos/Siniestros.dat", "ab");

    printf("\nIngrese numero de poliza o (-1) para finalizar: ");
    scanf("%d", &auxPoliza);

    while(auxPoliza != -1){
        valido = ValidarPoliza(auxPoliza);

        if (valido != -1){
            printf("Ingrese ID del siniestro: ");
            scanf("%d", &s.IDSiniestro);
            s.NroPoliza = auxPoliza;
            printf("Ingrese valor siniestrado $");
            scanf("%f", &s.ValorSiniestrado);

            fwrite(&s, sizeof(Siniestro), 1, Archivo);

        } else {
            printf("Error, la poliza ingresada no existe");
        }

        printf("\n\nIngrese numero de poliza o (-1) para finalizar: ");
        scanf("%d", &auxPoliza);
    }
    fclose(Archivo);
}

void CargaPoliza(FILE *Archivo){ //agrega un nuevo registro a Polizas.dat
    Poliza p;
    palabra auxApe;
    int valido;
    Archivo = fopen("archivos/Polizas.dat", "ab");

    printf("\nIngrese apellido del vendedor o (*) para finalizar: ");
    scanf("%s", auxApe);

    while (strcmp(auxApe, "*") != 0){
        valido = ValidarVendedor(auxApe);
        if (valido != -1){
            printf("Ingrese numero de poliza: ");
            scanf("%d", &p.NroPoliza);
            p.IDVendedor = valido;
            printf("Ingrese asegurado: ");
            scanf("%s", p.Asegurado);
            printf("Ingrese marca: ");
            scanf("%s", p.Marca);
            printf("Ingrese patente: ");
            scanf("%s", p.Patente);
            printf("Ingrese valor asegurado $");
            scanf("%f", &p.ValorAsegurado);

            fwrite(&p, sizeof(Poliza), 1, Archivo);
        } else {
            printf("Error, el vendedor ingresado no existe");
        }

        printf("\n\nIngrese apellido del vendedor o (*) para finalizar: ");
        scanf("%s", auxApe);
    }
    fclose(Archivo);
}

void CargaVendedor(FILE *Archivo){ //agrega un nuevo registro a Vendedores.dat
    Vendedor v;
    Archivo = fopen("archivos/Vendedores.dat", "ab");

    printf("\nIngrese ID del vendedor o (-1) para finalizar: ");
    scanf("%d", &v.IDVendedor);

    while( v.IDVendedor != -1){
        printf("Ingrese apellido del vendedor: ");
        scanf("%s", v.ApellidoVendedor);
        v.TasaRendimiento = 0;

        fwrite(&v, sizeof(Vendedor), 1, Archivo);

        printf("\nIngrese ID del vendedor o (-1) para finalizar: ");
        scanf("%d", &v.IDVendedor);
    }
    fclose(Archivo);
}

void CargarDatos(FILE *Archivo, int x){ //agrego datos al archivo correspondiente
    if (x == 1){ //vendedores
        CargaVendedor(Archivo);
    } else if (x == 2){ //polizas
        CargaPoliza(Archivo);
    } else { //siniestros
        CargaSiniestro(Archivo);
    }
}
