#ifndef DATOS_H_INCLUDED
#define DATOS_H_INCLUDED

const int N = 30, M = 8;
typedef char palabra[N];
typedef char stringpat[M];

struct Vendedor{
    int IDVendedor;
    palabra ApellidoVendedor;
    float TasaRendimiento;
};

struct Poliza{
    int NroPoliza;
    int IDVendedor;
    palabra Asegurado;
    palabra Marca;
    stringpat Patente;
    float ValorAsegurado;
};

struct Siniestro{
    int IDSiniestro;
    int NroPoliza;
    float ValorSiniestrado;
};

struct Asegurado{
    palabra NombreAsegurado;
    float TotalValorSiniestrado;
};

#endif // DATOS_H_INCLUDED
