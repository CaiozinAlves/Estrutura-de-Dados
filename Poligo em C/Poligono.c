#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float X;
    float Y;
} Ponto;

float AreaTri(Ponto A, Ponto B, Ponto C) {
    return fabs((A.X * B.Y + A.Y * C.X + B.X * C.Y) - (A.X * C.Y + A.Y * B.X + B.Y * C.X)) / 2.0;
}

float AreaPlg(int numVertices, Ponto *vertices) {
    float areaTotal = 0;
    Ponto A = vertices[0];
    for (int i = 1; i < numVertices - 1; i++) {
        Ponto B = vertices[i];
        Ponto C = vertices[i + 1];
        areaTotal += AreaTri(A, B, C);
    }
    return areaTotal;
}

int main() {
    FILE *arquivo;
    int numVertices;
    Ponto *vertices;

    arquivo = fopen("vertices.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fscanf(arquivo, "%d", &numVertices);
    vertices = (Ponto *)malloc(numVertices * sizeof(Ponto));

    for (int i = 0; i < numVertices; i++) {
        fscanf(arquivo, "%f %f", &vertices[i].X, &vertices[i].Y);
    }

    fclose(arquivo);

    float area = AreaPlg(numVertices, vertices);
    printf("A area do poligono e: %.2f\n", area);

    free(vertices);

    return 0;
}
