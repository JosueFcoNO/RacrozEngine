__kernel
void sum(__global int* OriginalNumber, __global int*NewNumber)
{
	int i = get_global_id(0); // Lo que regresa es el indice de la operaci�n que se est� ejecutando.

	NewNumber[i] = OriginalNumber[i] + i;
}
