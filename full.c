#include<stdio.h>
#include<stddef.h>   //because we use size_t variable, This is the unsigned integral type and is the result of the sizeof keyword.

char memory[25000];

struct block{
    size_t size;
    int free;
    struct block *next;
};

void initialize();
void split( struct block *fitting_slot, size_t size);
void *Mymalloc(size_t noofBytes);
void merge();
void MyFree(void *ptr);

struct block *freeList = (void*)memory;   //points to the first address of memory,still it hasn't a size

int main(){

    int *p = (int*)Mymalloc(100 * sizeof(int));    //type casting ,allocate 400 bytes
    char *q = (char*)Mymalloc(250 * sizeof(char));
    int *r = (int*)Mymalloc(1000 * sizeof(int));
    MyFree(p);

    char *s = (char*)Mymalloc(700);
    int *a = (int*)Mymalloc(100 * sizeof(int));
    MyFree(r);

   // int *a = (int*)Mymalloc(500* sizeof(int));
    int *b = (int*)Mymalloc(10000* sizeof(int));
    int *c;
    MyFree(c); 

    printf("\nAllocation & deallocation has done successfully");
}    

void initialize(){
    freeList -> size = 25000 -sizeof(struct block);
    freeList -> free = 1;
    freeList -> next = NULL;
}

void split(struct block *fitting_slot, size_t size){
    struct block * new =(void*)fitting_slot + size + sizeof(struct block);   //address of 1st block + 400 + 24 
    new -> size =(fitting_slot -> size) - size - sizeof(struct block);       //24976 - 400 -24 
    new -> free = 1;
    new -> next =fitting_slot ->next;
    fitting_slot ->size = size;
    fitting_slot ->free = 0;    //fitting_slot = 400 slot
    fitting_slot ->next = new;   //new= free list
}

void *Mymalloc(size_t noofBytes){  //noofBytes =400 
    struct block *curr;
    void *result;

    if(!(freeList -> size)){  //only run in 1st instruction as freelist hasn't assigned a size
        initialize();
        printf("\nMemory Intialization");
    }

    curr =freeList;    //size of curr=25000-24=24976 ,always curr take this value

    while((((curr -> size)< noofBytes) || ((curr ->free)==0)) && (curr -> next != NULL)){ //do not execute in 1st instruction, as this has no enough blocks to jump
        curr = curr -> next;  //run until we reach to the required slot
        printf("\nOne block checked");
    }
           //24976
    if( (curr -> size) == noofBytes){
        curr -> free =0;
        result =(void*)(++curr);
        printf("\nExact fitting block allocation");
        return result;
    }
         // 1st run =24976   ,2nd run = 24552
    else if((curr ->size)> (noofBytes + sizeof(struct block)) ){
        split (curr, noofBytes);        //seperate requested memory and new freelist
        result =(void*)(++curr);        // ++curr = 400  result= address of '400'
        printf("\nFitting block allocated with a split");
        return result;
    }

    else{
        result = NULL;
        printf("\nSorry,No sufficient memory to allocate");
        return result;
    }

}

void merge(){
    struct block *curr;
    curr = freeList;
    while( (curr-> next) != NULL){
        if( (curr -> free) && (curr -> next -> free)){
            curr -> size += (curr -> next -> size) + sizeof(struct block);
            curr -> next = curr -> next -> next;
        }
        curr = curr -> next;
    
    } 
     
    
}

void MyFree(void *ptr){
    if( ((void*)memory <= ptr) && (ptr <= (void*) (memory +25000))){
        struct block* curr = ptr;
        --curr;
        curr -> free = 1;
        merge(); //if there are two or more free slots together,we connect those slots to one slot
        printf("\nSuccessfully freed");
    }
    else{
        printf("\nPlease provide a valid pointer allocated by MyMalloc\n");
    }
}
     
