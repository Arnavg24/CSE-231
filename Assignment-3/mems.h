#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE 4096


typedef enum {
    HOLE=0,
    PROCESS=1
} NodeType;


typedef struct Nodeofsmolchain { //sub gucche constituents
    size_t size;
    NodeType type;
    struct Nodeofsmolchain *next;
    struct Nodeofsmolchain *prev;
} Nodeofsmolchain;


typedef struct Nodeofbigchain { //guccha node coverpage
    size_t size;//should be bytes count requirement
    int remainder;
    void *ptr; //the true pointer of big page "guccha" start
    void *virtual_address; // wannabe pointer value we assign and return from our wannabe malloc
    Nodeofsmolchain *subchain; //sub guccha linked list affiliated with this coverpage
    struct Nodeofbigchain *next;
    struct Nodeofbigchain *prev;
} Nodeofbigchain;

typedef struct intpair {
    int pages;
    int remainder;
} intpair;
// play around useful functions


// takes malloc input and splits into pages and remaining hole size
intpair get_pages(size_t size) {
    intpair pair;
    pair.pages = (int)size / PAGE_SIZE;
    pair.remainder = (PAGE_SIZE - (size % PAGE_SIZE))%PAGE_SIZE;
    if (pair.remainder != 0) {
        pair.pages++;
    }
    return pair;
}

// combine adjacent holes in sub gucche list
void combine_holes(Nodeofsmolchain *head) {
    Nodeofsmolchain *current = head;
    while (current != NULL) {
        if (current->type == HOLE && current->next != NULL && current->next->type == HOLE) {
            current->size += current->next->size ;
            current->next = current->next->next;
            if (current->next != NULL) {
                current->next->prev = current;
            }
            combine_holes(head);
        }
        current = current->next;
    }
}
//create gucche for a given main branch ( main branch has gucche  which may have type hole or process and must add up to allotted page size )
//use size attr of main branch and make linked list of gucche accordingly then link head of linked list to main node  -> subchain
void creategucche(Nodeofbigchain* head){//makes first sub gucche linked list for a given main branch original
//    printf("gucche creation called %u\n\n",head->virtual_address);
    void *smol_map_address = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);// link to chain of gucche of a coverpage
//    printf("gucche 2 creation called %u\n",head->virtual_address);
    Nodeofsmolchain *start_address;
    if (smol_map_address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
//    printf("gucche 3 creation called %u\n",head->virtual_address);
    start_address = (Nodeofsmolchain *)smol_map_address;
    start_address->size=(head->size)-(head->remainder);
    start_address->type=PROCESS;
    head->subchain=start_address;
    if(head->remainder!=0){
        void *next_smol_map_address = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);// link to next gucche element
        Nodeofsmolchain *new_address = (Nodeofsmolchain *)next_smol_map_address ;
        if (next_smol_map_address == MAP_FAILED) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
        start_address->next = new_address;
        start_address->prev = new_address;
        new_address->prev=start_address;
        new_address->size=head->remainder;
        new_address->type=HOLE;   //  insertion in linked list is probably fucked up fix later
//nm probably good enough
//        printf("69.guccha constructor size %u\n",new_address->size);
    }
}

void* start_address;
size_t adjusted_offset =1000; // Declaring adjusted_offset globally will be adjusted to account for highest allocated memory value right now


Nodeofbigchain* BIGhead = NULL;
int total_space_unused = 0;

void mems_init() {
    void *BIG_map_address = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);//cover of 1 page value contains all the segments made due to holes and processes in that page group (gucche of a cover may span over multiple but integral pages)

    if (BIG_map_address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    start_address = BIG_map_address;
    BIGhead = (Nodeofbigchain *)start_address;
    BIGhead->size = 0;//first page is not meant to be used
    BIGhead->remainder=0;
    BIGhead->ptr = start_address;
    BIGhead->virtual_address = (void*)adjusted_offset; // Assign the starting virtual address
    BIGhead->next = NULL;
    BIGhead->prev = NULL;
}
Nodeofbigchain* createBIGnode(int required_size){//takes input as needed size and inserts a new node into doubly linked list o big nodes also finds remainder to check for holes during later allocs


// allocate memory both for cover page as well as for the actual use case required ( whichever shit our wannabe malloc was called for )
    intpair pair = get_pages(required_size);
//    printf("\n\n\nfor size requirement %d, we make %d pages and %d freespace remains\n\n\n",required_size,pair.pages,pair.remainder);
    void* actuallyUSED_address = mmap(NULL, pair.pages*PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    void* bignode_map_address = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);// link to chain of gucche of a coverpage
    if (bignode_map_address == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    Nodeofbigchain* orighead = (Nodeofbigchain*)BIGhead;
    Nodeofbigchain *new_insertion_address = (Nodeofbigchain *)bignode_map_address;


//insert new_insertion_address into existing linked list with head as a global variable
    while(orighead->next!=NULL){
        orighead=orighead->next;
    }
    new_insertion_address->prev=orighead;
    orighead->next=new_insertion_address;

//set values
    new_insertion_address->ptr=actuallyUSED_address;
//    printf("constructor test actual ptr %u \n",new_insertion_address->ptr);
    new_insertion_address->size=PAGE_SIZE*pair.pages;
    new_insertion_address->remainder=pair.remainder;
    total_space_unused += (int)new_insertion_address->remainder;
    new_insertion_address->virtual_address=(void*) adjusted_offset;
//    printf("testing inside constructor %u and %u \n",adjusted_offset,new_insertion_address->virtual_address);
    adjusted_offset+=new_insertion_address->size;
// create the required gucche and link to attribute
    creategucche(new_insertion_address);
    return (new_insertion_address);
}

void *mems_malloc(size_t size) {
// start iterating over BIGhead's linked list
    void* virtual_add;
    Nodeofbigchain* cur_entry=BIGhead;
    while(cur_entry!=NULL){
        virtual_add=cur_entry->virtual_address;
//        printf("1. testing inside malloc %u and %u \n",cur_entry->remainder,size); //check mark
        if(cur_entry->remainder>=size){
//iterate over the gucche present and check if a single hole type guccha has size > size(required)
            Nodeofsmolchain* cur_guccha=cur_entry->subchain;
            while (cur_guccha!=NULL){
//                printf("2. testing inside malloc %u and %u \n",cur_guccha->size,size);
                if(cur_guccha->type==HOLE&&cur_guccha->size>=size){
//if yes then split the guccha into two and change the type of first guccha to process and return the virtual address of the first guccha
//for virtual address add coverpage's virtual address and add individual sizes of the gucchas while we are iterating through them
//also subtract size from remainder of coverpage
                    if(cur_guccha->size>size){
                        void *newlyformedguccha = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);// link to chain of gucche of a coverpage
                        Nodeofsmolchain *newstart;
                        if (newlyformedguccha == MAP_FAILED) {
                            perror("mmap");
                            exit(EXIT_FAILURE);
                        }
                        newstart = (Nodeofsmolchain *)newlyformedguccha;
                        newstart->size=cur_guccha->size-size;
                        newstart->type=HOLE;
//insert to linked list
                        newstart->next=cur_guccha->next;
                        newstart->prev=cur_guccha;
                        cur_guccha->next=newstart;}
//convert first segment of ex-hole to proc
                    cur_guccha->type=PROCESS;
                    cur_entry->remainder-=(int)size;
                    cur_guccha->size=size;//shouldnt do anything if the upper if condition isnt met
//                    printf("Virtual add testing in mems.h(found holes) %u",size);
                    total_space_unused -= (int)size;
                    return virtual_add;
                }
                virtual_add+=cur_guccha->size;
                cur_guccha=cur_guccha->next;
            }
        }
        cur_entry=cur_entry->next;
    }
    void* returnval = (createBIGnode((int)size)->virtual_address);
//    printf("Virtual add testing in mems.h(generating holes) %u",returnval);
    return  returnval;
}


void mems_print_stats() {
    int total_pages_used = 0;
    int main_chain_length = 0;
    if(BIGhead->next!=NULL) {
        Nodeofbigchain *current = BIGhead->next; // Start from the first actual node, not the initial head
        while (current != NULL) {
            printf("MAIN[%lu:%lu] -> ", (unsigned long) current->virtual_address,
                   (unsigned long) current->virtual_address + current->size - 1);
            Nodeofsmolchain *current_guccha = current->subchain;
            void *virtadd = current->virtual_address;
            while (current_guccha != NULL) {
                printf("<%s>[%lu:%lu] <-> ", current_guccha->type == HOLE ? "HOLE" : "PROCESS", (unsigned long) virtadd,
                       (unsigned long) virtadd + current_guccha->size - 1);
                virtadd += current_guccha->size;
                current_guccha = current_guccha->next;
            }
            printf("NULL\n");
            current = current->next;
        }

// Calculate statistics

        current = BIGhead->next; // Start from the first actual node, not the initial head
        int sub_chain_length[100]; // Adjust the array size accordingly
        int index = 0;
        while (current != NULL) {
            main_chain_length++;
            total_pages_used += (int) (current->size + PAGE_SIZE - 1) / PAGE_SIZE;
            Nodeofsmolchain *current_guccha = current->subchain;
            int subchain_length = 0;
            while (current_guccha != NULL) {
                subchain_length++;
                current_guccha = current_guccha->next;
            }
            sub_chain_length[index] = subchain_length;
            index++;
            current = current->next;
        }

// Print the statistics
        printf("Page used: %d\n", total_pages_used);
        printf("Space unused: %d\n", total_space_unused);
        printf("Main Chain Length: %d\n", main_chain_length);
        printf("Sub-chain Length array: ");
        for (int i = 0; i < main_chain_length; i++) {
            printf("%d ", sub_chain_length[i]);
        }
        printf("\n");
        return;
    }
    printf("Page used: %d\n", total_pages_used);
    printf("Space unused: %d\n", total_space_unused);
    printf("Main Chain Length: %d\n", main_chain_length);
    printf("Sub-chain Length array: ");
    printf("\n");
    printf("No memory allocated yet\n");
    return;
}


void mems_free(void *v_ptr) {
//    printf("%d",v_ptr);
    Nodeofbigchain *current = BIGhead;
    while (current != NULL) {
        if (current->virtual_address <= v_ptr && current->virtual_address + current->size > v_ptr) {
            Nodeofsmolchain *current_guccha = current->subchain;
            int virt_add = (int)current->virtual_address;
//            int testindex = 0;
            while (current_guccha != NULL) {
//                testindex++;
                if (current_guccha->type == PROCESS && virt_add <= (int)v_ptr && virt_add + current_guccha->size > (int)v_ptr) {
//                    printf("\n\n\n sdfdsfBDSFMNBFDSMNBSFDMNBSFDMNBEKUHFDWGIKUHBJHBJAAYQOIURAUSIBJANBBVJds\n\n\n");
//                    printf("Freeing %d bytes\n", (int)current_guccha->size);
//                    printf("current index %d v_ptr value:%d\n", testindex,v_ptr);
                    size_t freed_size = current_guccha->size;
                    current_guccha->type = HOLE;
                    total_space_unused += (int)freed_size;
                    current->remainder += (int)freed_size;
                    break;
                }
                current_guccha = current_guccha->next;
                virt_add += current_guccha->size;
            }
            combine_holes(current->subchain);
            break;
        }
        current = current->next;
    }
}

void *mems_get(void *v_ptr) {
    Nodeofbigchain *current = BIGhead;
    while (current != NULL) {
        if (current->virtual_address <= v_ptr && current->virtual_address + current->size > v_ptr){
            break;
        }
        current = current->next;
    }
    return (void*)current->ptr + ((size_t)v_ptr - (size_t)current->virtual_address);
}

void mems_finish() {
    Nodeofbigchain *current_big = BIGhead->next;
    while (current_big != NULL) {
// printf("check \n");
        Nodeofsmolchain *current_small = current_big->subchain;
        while (current_small != NULL) {
// printf("Check sub chain \n");
            Nodeofsmolchain *temp = current_small;
            current_small = current_small->next;
            munmap(temp, PAGE_SIZE);
        }
        Nodeofbigchain *temp_big = current_big;
        current_big = current_big->next;
        munmap(temp_big, PAGE_SIZE);
    }
    BIGhead->next = NULL;
    total_space_unused=0;
    adjusted_offset=1000;
}