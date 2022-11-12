#include <stdio.h>
#include <mpi.h>

// Primer preuzet sa https://mpitutorial.com/tutorials/introduction-to-groups-and-communicators/

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);
    // Get the rank and size in the original communicator
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    /**
     * Kreirati grupu od procesa iz komunikatora MPI_COMM_WORLD 
     * Funkcija MPI_Comm_group kreira grupu od procesa iz prosledjenog komunikatora
     */ 
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group); 

    int n = 7;
    const int ranks[7] = {1, 2, 3, 5, 7, 11, 13};

    /**
     * Funkcija MPI_Group_incl (skraćeno od "include") kreira novu grupu,
     * tako sto iz grupe prosledjene kao prvi parametar ukljucuje samo one procese
     * ciji se rank iz te grupe nalazi u nizu "ranks"
    */
    MPI_Group prime_group;
    MPI_Group_incl(world_group, 7, ranks, &prime_group);

    /**
     * Funkcija MPI_Comm_create_group kreaira novi komunikator od komunikatora prosledjenog kao prvi parametar.
     * U novom komunikatoru naci ce se svi procesi koji pripadaju grupi "prime_group". 
     * Ovo je grupna funkcija i moraju je izvrsiti svi procesi iz grupe "prime_group".
     * Novi komunikator se moze referencirati preko promenljive "prime_comm".
    */
    MPI_Comm prime_comm;
    MPI_Comm_create_group(MPI_COMM_WORLD, prime_group, 0, &prime_comm);

    int prime_rank = -1, prime_size = -1;
    /**
     * Promenljiva "prime_comm" bice jednaka MPI_COMM_NULL u svim procesima koji ne pripadaju ovom komunikatoru. 
     */
    if (MPI_COMM_NULL != prime_comm) {
        MPI_Comm_rank(prime_comm, &prime_rank);
        MPI_Comm_size(prime_comm, &prime_size);
    }

    printf("WORLD RANK/SIZE: %d/%d \t PRIME RANK/SIZE: %d/%d\n",
        world_rank, world_size, prime_rank, prime_size);

    MPI_Barrier(MPI_COMM_WORLD);

    if(world_rank == 0) {
        printf("\n\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int data, prime_root;
    if(world_rank == 1) {
        data = 100;
    }

    if (MPI_COMM_NULL != prime_comm) {
        MPI_Bcast(&data, 1, MPI_INT, 0, prime_comm);
    }
    printf("World rank = %d; prime rank = %d; data = %d\n", world_rank, prime_rank, data);

    MPI_Group_free(&world_group);

    if (MPI_COMM_NULL != prime_comm) {
        MPI_Group_free(&prime_group);
        MPI_Comm_free(&prime_comm);
    }

    MPI_Finalize();
}