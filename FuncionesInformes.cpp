#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Datos.h"
#include "FuncionesInformes.h"

float SumarCA(){ //lee Polizas.dat y realiza la sumatoria de todas las polizas que emitio la compañia
    float CapitalAsegurado = 0;
    Poliza auxP;
    FILE *Archivo;

    Archivo = fopen("archivos/Polizas.dat", "rb");

    fread(&auxP, sizeof(Poliza), 1, Archivo);

    while (!feof(Archivo)){
        CapitalAsegurado = CapitalAsegurado + auxP.ValorAsegurado;

        fread(&auxP, sizeof(Poliza), 1, Archivo);
    }

    fclose(Archivo);
    return CapitalAsegurado;
}

float SumarVS(int IDVendedor){ //lee Polizas.dat y Siniestros.dat y realiza la sumatoria de todos los pagos de siniestros
    float ValorSiniestrado = 0; //del empleado cuyo ID se pasa como parametro
    Poliza auxP;
    Siniestro auxS;
    FILE *ArchiPolizas;
    FILE *ArchiSiniestros;

    ArchiPolizas = fopen("archivos/Polizas.dat", "rb");
    fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);

    while (!feof(ArchiPolizas)){
        if (IDVendedor == auxP.IDVendedor){
            ArchiSiniestros = fopen("archivos/Siniestros.dat", "rb");
            fread(&auxS, sizeof(Siniestro), 1, ArchiSiniestros);

            while (!feof(ArchiSiniestros)){
                if (auxP.NroPoliza == auxS.NroPoliza){
                    ValorSiniestrado = ValorSiniestrado + auxS.ValorSiniestrado;
                }
                fread(&auxS, sizeof(Siniestro), 1, ArchiSiniestros);
            }
            fclose(ArchiSiniestros);
        }
        fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);
    }

    fclose(ArchiPolizas);
    return ValorSiniestrado;
}

float SumarVA(int IDVendedor){ //lee Polizas.dat y permite sumar los valores asegurados correspondientes al vendedor
    float ValorAsegurado = 0;      //cuyo ID se pasa como parametro
    Poliza auxP;
    FILE *Archivo;
    Archivo = fopen("archivos/Polizas.dat", "rb");

    fread(&auxP,sizeof(Poliza),1,Archivo);

    while (!feof(Archivo)){
        if (IDVendedor == auxP.IDVendedor){
            ValorAsegurado = ValorAsegurado + auxP.ValorAsegurado;
        }
        fread(&auxP,sizeof(Poliza),1,Archivo);
    }

    fclose(Archivo);
    return ValorAsegurado;
}

float CalcularTasaRendimiento(int IDVendedor){ //calculo de la tasa de rendimiento para el vendedor cuyo ID se envia como parametro
    float SumValorAsegurado, SumValorSiniestrado, TotalCapitalAsegurado, TasaRendimiento = 0;

    SumValorAsegurado = SumarVA(IDVendedor);
    SumValorSiniestrado = SumarVS(IDVendedor);
    TotalCapitalAsegurado = SumarCA();

    if (TotalCapitalAsegurado != 0){
        TasaRendimiento = ((SumValorAsegurado - SumValorSiniestrado) * 100)/TotalCapitalAsegurado;
    }

    return TasaRendimiento;
}

void Rendimientos(FILE *Archivo){ //recorre el archivo Vendedores.dat y llama a CalcularTasaRendimiento para reasignar cada
    Vendedor auxV;                  //vendedor con su porcentaje de rendimiento de las polizas vendidas antes de hacer informes
    int nReg, cont = 0;
    Archivo = fopen("archivos/Vendedores.dat", "rb+"); //Abre el archivo binario para lectura/escritura.
    fseek(Archivo,0,SEEK_END);
    nReg = ftell(Archivo)/sizeof(Vendedor);

    while (cont < nReg){
            fseek(Archivo,(cont*sizeof(Vendedor)),SEEK_SET);
            fread(&auxV,sizeof(Vendedor),1,Archivo);
            auxV.TasaRendimiento = CalcularTasaRendimiento(auxV.IDVendedor);
            fseek(Archivo,(cont*sizeof(Vendedor)),SEEK_SET);
            fwrite(&auxV,sizeof(Vendedor),1,Archivo);
            cont++;
    }

    fclose(Archivo);
}

void MenuInforme(int *p){   //muestra posibles informes y valida que se seleccione una opcion correcta
    printf("\nINFORMES:");
    printf("\n   1. Archivos");
    printf("\n   2. Polizas por vendedor");
    printf("\n   3. Vendedores a premiar");
    printf("\n   4. Asegurado piedra");
    printf("\nOpcion: ");
    scanf("%d", p);
    while ((*p < 1) || (*p >4)){
        printf("Opcion invalida, reingrese: ");
        scanf("%d", p);
    }
}

void ListarInfo(FILE *Archivo){ //muestra el contenido de los archivos
    int x;
    printf("\nARCHIVOS:");
    printf("\n   1. Vendedores");
    printf("\n   2. Polizas");
    printf("\n   3. Siniestros");
    printf("\nSeleccione informe: ");
    scanf("%d", &x);
    while ((x < 1) || (x >3)){
        printf("Opcion invalida, reingrese: ");
        scanf("%d", &x);
    }
    if (x == 1){
        Vendedor auxV; //no esta declarado al comienzo de la funcion porque dependiendo de la opcion seleccionada va a ser necesaria o no
        printf("\n\nVENDEDORES:\n");
        printf("--------------------------");
        Archivo = fopen("archivos/Vendedores.dat", "rb");
        fread(&auxV, sizeof(Vendedor), 1, Archivo);

        while (!feof(Archivo)){
            printf("\nVendedor ID %d \t%s",auxV.IDVendedor,auxV.ApellidoVendedor);
            printf("\nTasa de rendimiento %.1f%%",auxV.TasaRendimiento);
            printf("\n--------------------------");
            fread(&auxV, sizeof(Vendedor), 1, Archivo);
        }
        fclose(Archivo);
    } else if (x == 2){
        Poliza auxP; //no esta declarado al comienzo de la funcion porque dependiendo de la opcion seleccionada va a ser necesaria o no
        printf("\n\nPOLIZAS:\n");
        printf("------------------------------------------------------");
        Archivo = fopen("archivos/Polizas.dat", "rb");
        fread(&auxP, sizeof(Poliza), 1, Archivo);

        while (!feof(Archivo)){
            printf("\nNumero de poliza %d \tID vendedor %d",auxP.NroPoliza,auxP.IDVendedor);
            printf("\nAsegurado %s \tMarca %s \tPatente %s",auxP.Asegurado,auxP.Marca,auxP.Patente);
            printf("\nValor asegurado $%.2f",auxP.ValorAsegurado);
            printf("\n------------------------------------------------------");
            fread(&auxP, sizeof(Poliza), 1, Archivo);
        }
        fclose(Archivo);
    } else {
        Siniestro auxS; //no esta declarado al comienzo de la funcion porque dependiendo de la opcion seleccionada va a ser necesaria o no
        printf("\n\nSINIESTROS:\n");
        printf("---------------------------------------------------");
        Archivo = fopen("archivos/Siniestros.dat", "rb");
        fread(&auxS, sizeof(Siniestro), 1, Archivo);

        while (!feof(Archivo)){
            printf("\nID siniestro %d \tNumero de poliza %d",auxS.IDSiniestro,auxS.NroPoliza);
            printf("\nValor siniestrado $%.2f",auxS.ValorSiniestrado);
            printf("\n---------------------------------------------------");
            fread(&auxS, sizeof(Siniestro), 1, Archivo);
        }
        fclose(Archivo);
    }
}

void ListarPolizasVendedor(palabra ApellidoVendedor){ //muestra todas las polizas y todos los siniestros del vendedor cuyo
    int aux = 0, cont;                                  //apellido es pasado como parametro
    FILE *Archivo;
    FILE *Archivo2;
    Vendedor auxV;
    Poliza auxP;
    Siniestro auxS;
    //valido que el vendedor este registrado,si esta guardo en aux su ID para poder buscarlo en Poliza.dat
    Archivo = fopen("archivos/Vendedores.dat", "rb");
    fread(&auxV, sizeof(Vendedor), 1, Archivo);
    while (!feof(Archivo)){
        if (strcmp(auxV.ApellidoVendedor,ApellidoVendedor) == 0){
            aux = auxV.IDVendedor;
        }
        fread(&auxV, sizeof(Vendedor), 1, Archivo);
    }
    fclose(Archivo);

    if (aux == 0){
        printf("\nEl apellido ingresado no corresponde a un vendedor");
    } else {
        printf("\nPolizas y siniestros asociados al vendedor %s:",ApellidoVendedor);
        //busco las polizas del vendedor
        cont = 0;
        Archivo = fopen("archivos/Polizas.dat", "rb");
        fread(&auxP, sizeof(Poliza), 1, Archivo);
        while (!feof(Archivo)){
            if (aux == auxP.IDVendedor){
            printf("\n - Poliza Nro %d",auxP.NroPoliza);
            //busco los siniestros del vendedor para esa poliza
            Archivo2 = fopen("archivos/Siniestros.dat", "rb");
            fread(&auxS, sizeof(Siniestro), 1, Archivo2);
            while (!feof(Archivo2)){
                if (auxP.NroPoliza == auxS.NroPoliza){
                    printf("\n - ID Siniestro %d",auxS.IDSiniestro);
                }
                fread(&auxS, sizeof(Siniestro), 1, Archivo2);
            }
            fclose(Archivo2);
            cont = 1; //variable bandera que me sirve para evaluar si no se encontro ninguna poliza para asociada al vendedor
            }
        fread(&auxP, sizeof(Poliza), 1, Archivo);
        }
        fclose(Archivo);
        if (cont == 0){ //si cont == 0 el vendedor no tiene polizas asociadas, como no se puede cargar un siniestro si la poliza no existe
            printf("\n  No registra polizas ni siniestros");  //ese vendedor tampoco tiene siniestros asociados.
        }
    }
}

void VendedoresAPremiar(){
    int aux = 0;
    Vendedor auxV;
    FILE *Archivo;
    Archivo = fopen("archivos/Vendedores.dat", "rb");
    printf("\nVendedores a premiar:");

    fread(&auxV, sizeof(Vendedor), 1, Archivo);
    while (!feof(Archivo)){
        if (auxV.TasaRendimiento > 5){
            printf("\n - Vendedor %s, ID %d",auxV.ApellidoVendedor,auxV.IDVendedor);
            printf("\n\tTasa de rendimiento %.1f%%",auxV.TasaRendimiento);
            aux = 1; //variable bandera que me sirve para evaluar si hay empleados a premiar
        }
        fread(&auxV, sizeof(Vendedor), 1, Archivo);
    }
    fclose(Archivo);
    if (aux == 0){ //si aux == 0 no hay vendedores con tasa de rendimiento > 5%
        printf("\n No hay vendedores con una tasa de rendimiento mayor al 5%%");
    }
}

float CalcularValorSiniestradoTotal(palabra NombreAsegurado){ //busco las polizas del asegurado, y los siniestros correspondientes a esas polizas
    float ValorSiniestradoT = 0;                                //para calcular el valor total siniestrado que se le pago al asegurado cuyo nombre
    FILE *ArchiPolizas;                                             //se paso como parametro
    FILE *ArchiSiniestros;
    Poliza auxP;
    Siniestro auxS;
    ArchiPolizas = fopen("archivos/Polizas.dat", "rb");
    fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);
    while(!feof(ArchiPolizas)){
        if (strcmp(auxP.Asegurado,NombreAsegurado) == 0){
            ArchiSiniestros = fopen("archivos/Siniestros.dat", "rb");
            fread(&auxS, sizeof(Siniestro), 1, ArchiSiniestros);
            while(!feof(ArchiSiniestros)){
                if (auxP.NroPoliza == auxS.NroPoliza){
                    ValorSiniestradoT = ValorSiniestradoT + auxS.ValorSiniestrado;
                }
                fread(&auxS, sizeof(Siniestro), 1, ArchiSiniestros);
            }
            fclose(ArchiSiniestros);
        }
         fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);
    }
    fclose(ArchiPolizas);
    return ValorSiniestradoT;
}

void AseguradoPiedra(){ //muestra toda la informacion disponible del asegurado al que mas dinero debio pagarsele por siniestros
    FILE *ArchiAsegurados;
    FILE *ArchiPolizas;
    Asegurado auxA, Amax;
    Poliza auxP;
    int aux,nReg;
    //como un asegurado puede tener uno o mas registros cargo en un nuevo archivo todos los asegurados que figuran en las polizas
    ArchiPolizas = fopen("archivos/Polizas.dat", "rb");
    fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);
    while(!feof(ArchiPolizas)){
        //Busco los asegurados
        aux = 0;
        ArchiAsegurados = fopen("archivos/Asegurados.dat", "rb+");
        fread(&auxA, sizeof(Asegurado), 1 , ArchiAsegurados);
        while (!feof(ArchiAsegurados) && (aux == 0)){
            if (strcmp(auxP.Asegurado,auxA.NombreAsegurado) == 0){ //busco si el asegurado ya esta registrado en Asegurados.dat
                aux = 1; //esta, por lo que finalizo la busqueda
            }
            fread(&auxA, sizeof(Asegurado), 1 , ArchiAsegurados);
        }

        if (aux == 0){ //no está, asi que debo agregarlo
                strcpy(auxA.NombreAsegurado, auxP.Asegurado);
                auxA.TotalValorSiniestrado = 0; //inicializo en 0, luego sera calculado este valor
                fseek(ArchiAsegurados,0,SEEK_END); //me ubico al final del archivo
                fwrite(&auxA,sizeof(Asegurado),1,ArchiAsegurados);
            }
        fclose(ArchiAsegurados);
        fread(&auxP, sizeof(Poliza), 1, ArchiPolizas);
    }
    fclose(ArchiPolizas);

    //teniendo todos los asegurados cargados en Asegurados.dat busco cuando dinero se le pago a cada uno
    Amax.TotalValorSiniestrado = 0; //voy a utilizar Amax para determinar el maximo
    ArchiAsegurados = fopen("archivos/Asegurados.dat", "rb+");
    fseek(ArchiAsegurados,0,SEEK_END);
    nReg = ftell(ArchiAsegurados)/sizeof(Asegurado);
    aux = 0;
    while (aux < nReg){ //sobreescribo los registros con el valor total siniestrado correcto
            fseek(ArchiAsegurados,(aux*sizeof(Asegurado)),SEEK_SET);
            fread(&auxA,sizeof(Asegurado),1,ArchiAsegurados);
            auxA.TotalValorSiniestrado = CalcularValorSiniestradoTotal(auxA.NombreAsegurado);
            fseek(ArchiAsegurados,(aux*sizeof(Asegurado)),SEEK_SET);
            fwrite(&auxA,sizeof(Asegurado),1,ArchiAsegurados);
            aux++;
            if (auxA.TotalValorSiniestrado > Amax.TotalValorSiniestrado){ //maximo
                Amax = auxA;
            }
    }
    printf("\n\nEl asegurado piedra es %s",Amax.NombreAsegurado);
    printf("\nSe le pago un total del $%.2f por siniestros",Amax.TotalValorSiniestrado);
    fclose(ArchiAsegurados);
}
