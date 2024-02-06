
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRY do {
#define CATCH(error) } while(0); \
                    goto error; \
                    error:


#define ADMINISTRATIVO 1
#define OPERARIO 2

typedef struct {
    char nombre[50];
    int num_id;
    float salario_base;
    int tipo_empleado; // 1 = administrativo, 2 = operario
    union {
        int antiguedad; // Años trabajados para administrativos
        int produccion; // Unidades producidas para operarios
    }extra;
} Empleado;

Empleado newEmpleado(const char* nombre, int num_id, float salario_base, int tipo_empleado, int extra) {
    Empleado empty = {"", 0, 0.0, 0, {0}}, emp;

    TRY //do {
        strcpy(emp.nombre, nombre);
        emp.num_id = num_id;
        emp.salario_base = salario_base;
        emp.tipo_empleado = tipo_empleado;
        if (tipo_empleado == ADMINISTRATIVO) {
            emp.extra.antiguedad = extra;
        } else if (tipo_empleado == OPERARIO) {
            emp.extra.produccion = extra;
        } else {
            printf("Error: Tipo de empleado no válido.\n");
            goto error;
        }
        goto success;
    CATCH(error) //} while(0);\goto error;\error:
        {
            return empty;
        }
        success:
        return emp;

}
float calcularSalario(const Empleado* emp) {
    float salario_total = emp->salario_base;
    if (emp->tipo_empleado == ADMINISTRATIVO)
        salario_total += emp->salario_base * 0.05 * emp->extra.antiguedad;

    if (emp->tipo_empleado == OPERARIO)
        salario_total += emp->salario_base * 0.02 * emp->extra.produccion;

    return salario_total;
}

int main()
{
    Empleado emp1 = newEmpleado("Juan", 1, 1500.10, ADMINISTRATIVO, 5);
    Empleado emp2 = newEmpleado("Pedro", 2, 1200.10, OPERARIO, 1000);

    printf("Salariooo de %s: %.2f\n", emp1.nombre, calcularSalario(&emp1));
    printf("Salario de %s: %.2f\n", emp2.nombre, calcularSalario(&emp2));

    return 0;
}
