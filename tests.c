#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "mymalloc.h"

char *globStr = NULL;

/* La fonction d'initialisation de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int init_suite1(void) {
    //On cherche à allouer un bloc mémoire qui sera utilisé dans nos tests
    globStr = malloc(sizeof(char)*20);
    if (globStr == NULL)
        return -1;

    return 0;
}

/* La fonction de "nettoyage" de la suite de tests.
* Retourne 0 en cas de succès, n'importe quelle autre valeur indique un échec.
*/
int clean_suite1(void) {
    free(globStr); //on libère la mémoire allouée lors de l'initialisation de la suite
    return 0;
}
struct bloc_header *tempa;

// @tri:test01 => [Allocation 1ere bloc dont la taille devrait être 8]
void test01(void) {
    struct bloc_header *a = mymalloc(sizeof(struct bloc_header));
    CU_ASSERT_EQUAL((a-1)->size, 8);
    tempa=a;
}
// @tri:test02 => [Allocation 2eme bloc dont la taille devrait être 12]
void test02(void) {
    struct bloc_header *c = mymalloc(2*sizeof(struct bloc_header));
    CU_ASSERT_EQUAL((c-1)->size, 12); 
}
// @tri:test03 => [Allocation 3eme bloc dont la taille devrait être 28]
void test03(void) {
    int *f = mymalloc(21);
    struct bloc_header *g = (struct bloc_header*)f-1;
    CU_ASSERT_EQUAL(g->size, 28); 
}
// @tri:test04 => [Allocation 4eme bloc dont la taille devrait être 52 (heap full 100)]
void test04(void) {
    int *l = mymalloc(48);
    struct bloc_header *m = (struct bloc_header*)l-1;
    CU_ASSERT_EQUAL(m->size, 52); 
}
// @tri:test05 => [Le 1ere bloc devrait etre toujorus alloué et avoir une taille de 8]
void test05(void) {
    CU_ASSERT_EQUAL((temp-1)->alloc, 1); 
    CU_ASSERT_EQUAL((temp-1)->size, 8); 
}
// @tri:test06 => [Le 1ere bloc ne devrait plus etre alloué après un free]
void test06(void) {
    myfree(tempa);
    CU_ASSERT_EQUAL((tempa-1)->alloc, 0);
}
// @tri:test07 => [LA 1ere bloc devrait être réallouer si on demande la meme taille]
void test06(void) {
    int *x=mymalloc(sizeof(struct bloc_header));
    CU_ASSERT_EQUAL((x-1)->alloc, 1);
    CU_ASSERT_EQUAL((x-1)->size, 8);
}



int main() {
    CU_pSuite pSuite = NULL;

    /* On initialise la suite de tests */
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* On ajoute la suite au registre */
    pSuite = CU_add_suite("Suite de tests pour le projet MyMalloc", init_suite1, clean_suite1);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* On ajoute les tests à la suite. L'ordre est important !
     * test_strlen1 sera exécuté en premier, puis test_strlen2, etc ...*/
    if(NULL == CU_add_test(pSuite, "test01", test01) ||
       NULL == CU_add_test(pSuite, "test02", test02) ||
       NULL == CU_add_test(pSuite, "test03", test03) ||
       NULL == CU_add_test(pSuite, "test04", test04) ||
       NULL == CU_add_test(pSuite, "test05", test05) ||
       NULL == CU_add_test(pSuite, "test06", test06)
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    /* On exécute les tests et on vide ensuite la mémoire utilisée par CUnit */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
