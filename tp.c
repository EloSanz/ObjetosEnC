#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMINISTRATIVO 1
#define OPERARIO 2
#define TRY //No hace nada
#define CATCH(error) if(error) //setear error a true manualmente

typedef float (*CalcularSalario)(void*);

typedef struct {
    int produccion; // Unidades producidas para operarios
} Operario;

typedef struct {
    int antiguedad; // Años trabajados para administrativos
} Administrativo;

typedef struct {
    char nombre[50];
    int num_id;
    float salario_base;
    int tipo_empleado;
    union {//union reserva memoria compartida para uno, sirve para ahorrar memoria
        Operario operario_data;
        Administrativo administrativo_data; 
    }extra;
    CalcularSalario calcularSalario; 
}Empleado;

float calcularSalario(const Empleado* emp) {
    emp->calcularSalario; //apunta a alguno de los siguientes de abajo, si fallo el tipoEmpleado, va por defecto a calcularSalarioOperario
}   
float calcularSalarioAdministrativo(void* emp) {
    Empleado* administrativo = (Empleado*)emp;
    return administrativo->salario_base + (administrativo->salario_base * 0.05 * administrativo->extra.administrativo_data.antiguedad);
}
float calcularSalarioOperario(void* emp) {
    Empleado* operario = (Empleado*)emp;
    int error = 0;
    float salarioOperario = operario->salario_base;
    TRY 
        if(salarioOperario == -1)
              error = 1;
        else
            salarioOperario +=(operario->salario_base * 0.02 * operario->extra.operario_data.produccion);
    CATCH(error)
    {
        printf("\nExcepcion, calculando salario de Empleado tipo invalido.\nSe retornara un -1.00\n");
    }
        return salarioOperario;
}

Empleado newEmpleado(const char* nombre, int num_id, float salario_base, int tipo_empleado, int extra) {
    Empleado empty = {"null", -1, -1.0, 0, {0}}, emp;
    int error = 0;
    TRY 
        strcpy(emp.nombre, nombre);
        emp.num_id = num_id;
        emp.salario_base = salario_base;
        emp.tipo_empleado = tipo_empleado;
        if (tipo_empleado == ADMINISTRATIVO) {
            emp.calcularSalario = calcularSalarioAdministrativo;
            emp.extra.administrativo_data.antiguedad = extra;
        } 
        else if (tipo_empleado == OPERARIO) {
            emp.calcularSalario = calcularSalarioOperario;
            emp.extra.operario_data.produccion = extra;
        } 
        else {
            error = 1;
        }
    CATCH(error) //if(var)
        {
            empty.calcularSalario = calcularSalarioOperario; // por defecto
            return empty;
        }
        return emp; 
    
}
Empleado newAdministrativo(const char nombre[], int num_id, float salario_base, int antiguedad) {
    Empleado emp = {"", num_id, salario_base, ADMINISTRATIVO, {antiguedad}};
    emp.calcularSalario = calcularSalarioAdministrativo; // Asignación del puntero a funcion
    strcpy(emp.nombre, nombre);
    return emp;
}
Empleado newOperario(const char nombre[], int num_id, float salario_base, int produccion) {
    Empleado emp = {"", num_id, salario_base, OPERARIO,{produccion}};
    strcpy(emp.nombre, nombre);
    emp.calcularSalario = calcularSalarioOperario; // Asignación del puntero a funcion
    return emp;
}
//


int main()
{
            //Empleado newEmpleado(nombre,  num_id,  salario_base,  tipo_empleado, int extra) 
                                                                        //este 13 está erróneo en tipo Empleado
    Empleado emp1 = newEmpleado("Juan el empleado ADMINISTRATIVO", 1, 100, 13, 1000);
    Empleado emp2 = newEmpleado("Pedro el empleado OPERARIO"     , 2, 100, OPERARIO, 1000);

    Empleado admi_1 = newAdministrativo("Juan el Admin", 3, 100, 1000);
    Empleado ope_1 = newOperario("Pedro el Operario", 4, 100, 1000);

   
    printf("Empleado id: %d Salario de %s: %.2f\n",emp1.num_id, emp1.nombre, emp1.calcularSalario(&emp1));//la unica manera de simular this.calcularSalario
    printf("Empleado id: %d Salario de %s: %.2f\n",emp2.num_id, emp2.nombre, emp2.calcularSalario(&emp2));
    printf("Empleado id: %d Salario de %s: %.2f\n",admi_1.num_id ,admi_1.nombre, admi_1.calcularSalario(&admi_1));//la unica manera de simular this.calcularSalario
    printf("Empleado id: %d Salario de %s: %.2f\n",ope_1.num_id ,ope_1.nombre, ope_1.calcularSalario(&ope_1));
    return 0;
}