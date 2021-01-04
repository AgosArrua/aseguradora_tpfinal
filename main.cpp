#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Datos.h"
#include "FuncionesCarga.h"
#include "FuncionesInformes.h"

int main()
{
    int exit = 1, opcion, carga, informe;
    palabra AuxApellido;
    FILE *Archivo;

    VerificarArchivos();

    while (exit){
        Menu();
        printf("\nOpcion: ");
        scanf("%d", &opcion);

        switch (opcion){
            case 1: //carga
                MenuCarga(&carga);
                CargarDatos(Archivo, carga);
                break;
            case 2: //informes
                Rendimientos(Archivo);
                MenuInforme(&informe);
                if (informe == 1){ //listar informacion de los archivos
                    ListarInfo(Archivo);
                } else if (informe == 2){ //listar polizas de un vendedor
                    printf("\nIngrese el apellido del vendedor: ");
                    scanf("%s", AuxApellido);
                    ListarPolizasVendedor(AuxApellido);
                } else if (informe == 3){ //vendedores a premiar
                    VendedoresAPremiar();
                } else { //asegurado piedra
                    AseguradoPiedra();
                }
                break;
            case 0: //salir
                exit = 0;
                break;
            default:
                printf("La opcion ingresada no es valida");
        }
    }

    printf("\n\n\tFin del programa\n  ");
    system("pause");
    return 0;
}
