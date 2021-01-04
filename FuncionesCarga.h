#ifndef FUNCIONESCARGA_H_INCLUDED
#define FUNCIONESCARGA_H_INCLUDED

void VerificarArchivos();
void Menu();
void MenuCarga(int *);
void CargarDatos(FILE *, int);
void CargaVendedor(FILE *);
void CargaPoliza(FILE *);
void CargaSiniestro(FILE *);
int ValidarVendedor(palabra);
int ValidarPoliza(int);

#endif // FUNCIONESCARGA_H_INCLUDED
