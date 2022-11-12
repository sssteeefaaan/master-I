void assign(int *a, int *b, int size) {
    #pragma acc kernels
    {
        for (int i = 0; i < size - 1; i++)
            a[i] = b[i + 1];
    }
}

int main() {
    return 0;
}