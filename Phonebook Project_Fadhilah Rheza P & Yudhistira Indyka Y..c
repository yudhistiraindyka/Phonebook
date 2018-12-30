#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

typedef struct contact 
{   
    int number;        /*unique account number*/
    char name[20];     /*mengandung name*/  
    char phone[15];    /*mengandung No. HP*/
    char email[20];           /*mengandung email address*/
    struct contact *next; /*next digunakan untuk menavigasi melalui struktur.*/ 
    int count;     /*count digunakan untuk menginput array*/    
    
} Contact;
Contact *firstc,*currentc,*newc; /*pointers*/
/*firstc digunakan untuk menunjukkan first record dalam list*/
/*currentc menunjukkan current record dalam list*/
/*newc mengandung address structure/node baru*/
int cnum = 0; /*unique contact number*/

void clearInput(void);
void addNewcontact(void);
void list(void);
void del(void);
void edit(void);
int find(void);
int prompt(void);
int findnum (int);

int main()
{
    FILE *datafile;
    char *filename = "kontak.dat";/*mendeclare file name*/
    char ch;
    firstc = NULL;
    datafile = fopen(filename,"r");/*membuka file untuk dibaca*/
    
    if(datafile)	 
    {
	    firstc = (struct contact *)malloc(sizeof(struct contact)); /*use of malloc to set aside memory relative to size of structure contact*/
	    currentc = firstc;       /*make first record current*/
	    while(1) /*endless while loop. a NULL pointer in final node ends loop*/
     	{
			newc = (struct contact *)malloc(sizeof(struct contact));
			fread(currentc,sizeof(struct contact),1,datafile);
		    if(currentc->next == NULL)   /* NULL indicates end of node list*/
			break;
		    
			currentc->next = newc;       /* pointer referencing next node*/
            currentc->count=0;           /* initiates count for comments*/
		    currentc = newc;             /* make current record new*/
	    }
	    fclose(datafile);                /* close file - good practice to cloe files after use*/
	    cnum = currentc->number; 
    }
        
    do
    {
		fflush(stdin);
		printf("================");
        printf("\nContact Database");//print menu messages
        printf("\n================");
        printf("\n1 - Menambah kontak");     
		printf("\n2 - Menghapus kontak");          
		printf("\n3 - Menampilkan daftar kontak");       
		printf("\n4 - Meng-edit kontak");          
		printf("\n5 - Mencari kontak berdasarkan nama");  
		printf("\n--------------------------------");
        printf("\nQ - Save and quit\n");         
        printf("\nPilihan anda: ");            
        ch = getchar();
	    ch = toupper(ch);//changes user input case to upper case
        switch(ch)     //stores in ch variable
        {
        	case '1':
            printf("\nMenambah kontak\n");
	        fflush(stdin);
            addNewcontact();//call addNewcontact function 
            break;
	    	
			case '2':
			printf("\nMenghapus kontak\n");
			del();
			break;
	    
			case '3':
			printf("\nDaftar kontak\n");
			system("cls");
			list();
			break;
	    
			case '4':
			printf("\nEdit kontak\n");
			edit();
			break;
	    
			case '5':
			printf("\nMencari kontak berdasarkan nama\n");
			find();
			break;
            
			case 'Q':
            printf("\nSave and quit\n");
            default:
            break;
        }
    }
    while(ch != 'Q');

    /* Save the records to disk*/
    currentc = firstc;
    
    if(currentc == NULL)
	return(0);		/*no data to write*/
				  
    datafile = fopen(filename,"w");   /*open file to write*/
    
    if(datafile == NULL)
    {
	    printf("Error writing to %s\n",filename);
	    return(1);
    }
    
	/* Write each record to disk*/ 
    while(currentc != NULL)
    {
	    fwrite(currentc,sizeof(struct contact),1,datafile);
	    currentc = currentc->next;
    }
    fclose(datafile);             /*closes data file*/
    return(0);
}

void addNewcontact(void) /*add new contact function*/
{
    newc = (struct contact *)malloc(sizeof(struct contact)); /*allocates memory for new structure.*/
/*
 * Cek untuk melihat apakah ini adalah first record dalam file
 * jika iya, maka semua pointer akan initialized ke record ini,
 */
    if(firstc==NULL)
        firstc = currentc = newc;
/* 
 * Jika enggak, end of structure list is obtained
 */
    else
    {
        currentc = firstc;      /*membuat first record menjadi curent record*/ 
                                
        while(currentc->next != NULL)currentc = currentc->next;
                                /*loop melalui semua record*/ 
        currentc->next = newc;  /*pointer ke node selanjutnya*/
        currentc = newc;        /*membuat current record menjadi new record*/ 
    }

	/* update struktur */
    cnum++;
    printf("%27s: %5i\n","#Kontak",cnum);
    currentc->number = cnum;    /*cnum sebagai nomor kontak*/
    
    printf("%27s: ","Masukkan nama");
    gets(currentc->name);
    printf("%27s: ","Masukkan no. telpon");
    gets(currentc->phone);
    printf("%27s: ","Masukkan email");
    gets(currentc->email);
    system("cls");
    printf("\nKontak berhasil ditambahkan!\n\n");
    currentc->count=0;
/* 
 * memberikan new record NULL pointer
 * untuk menunjukkan ini adalah last record:
 */
    currentc->next = NULL;
}

void list(void)	/* list semua fungsi kontak*/
{
    if(firstc==NULL)
        puts("Daftar kontak kosong");
        
    else
    {
	    printf("%6s %-20s %-15s %-15s\n","#Kontak","Nama","Telpon","Email");
        puts("------- -------------------- -------------   -------------------");           
        currentc=firstc;
        do
        {
        	printf("%6d: %-20s %-15s %-20s\n", currentc->number, currentc->name, currentc->phone, currentc->email); /*print nama, nomor, dan email*/
        }
        
        while((currentc=currentc->next) != NULL);
    }
}

void del(void)	/*fungsi delete kontak*/          
{
    int record;
    struct contact *previousa;
    if(firstc==NULL)
    {
    puts("Tidak ada kontak untuk dihapus");
	return;
    }
    
    list();	/*menunjukkan semua records*/  
    printf("Masukkkan #Kontak yang ingin dihapus: ");
    scanf("%d",&record);
    currentc = firstc;
    system("cls");
    while(currentc != NULL)
    {
        if(currentc->number == record)
	{
	    if(currentc == firstc)	/*if record to be deleted is the first record*/
		firstc=currentc->next; /*reset firstc to point at next record as first*/
	    else
		previousa->next = currentc->next;/*previous pointer used if record*/ 
                                         /*to delete is not the first*/
        free(currentc); /*frees memory <deletes>*/
	    printf("\nKontak #%d berhasil dihapus!\n\n",record);
	    return;
	}
	
	else
	{
        previousa = currentc;
	    currentc = currentc->next;
	}
    }
    printf("\nKontak #%d tidak ditemukan!\n\n",record);
 }

void edit(void)		/*fungsi mengedit kontak*/
{
    int record, result;
    if(firstc==NULL)
    {
        puts("Tidak ada kontak yang bisa diubah");
	return;
    }
    
    list();		/* show all recorded contacts */
    printf("Masukkan #Kontak yang ingin diubah: ");
    scanf("%d",&record);  /*scan user input to record*/
    result = findnum(record);
    
    if( result >0 ){
	    printf("Kontak #%d:\n",currentc->number);
	    printf("Nama: %s\n",currentc->name);
	    if(prompt())
		    gets(currentc->name);
	    printf("No. Telpon: %s\n",currentc->phone);
	    if(prompt())
		    gets(currentc->phone);
	    printf("Email: %s\n",currentc->email);
	    if(prompt())
		    gets(currentc->email);
	    return;
	}
	system("cls");
    printf("\nKontak #%d tidak ditemukan!\n\n",record);
}

int findnum (int recordnum)
{
    int record;
    record = recordnum;
    currentc																																							 = firstc;
    while(currentc != NULL)
    {
                   
        if(currentc->number == record)
	    {
           return 1;
        }
		 
		else
		{
	    currentc = currentc->next;
		}
    }
    return -1;   
}

int find(void) /* find contact function*/
{
     char buff[20];
     
     if(firstc==NULL)
	{
        puts("Tidak ada kontak untuk dicari!");
	    return 1;
    }
    system("cls");
    printf("Masukkan nama kontak yang ingin dicari: ");
    fflush(stdin);/*clears any text from the input stream*/
    gets(buff);
    
    currentc = firstc;
    while(currentc != NULL)
    {
        if( strcmp(currentc->name, buff) == 0 )
	    {
			printf("%6s %-20s %-15s %-15s\n","#Kontak","Nama","Telepon","Email");/*prints table titles*/
            printf("%6d: %-20s %-15s %-20s\n",currentc->number,currentc->name,currentc->phone,currentc->email); /*prints values of number, name, phone and email*/
			
			return 0;
	    }
		else
		{
			currentc = currentc->next;
		}
    }
    system("cls");
    printf("\nKontak %s tidak ditemukan!\n\n",buff);
    return 1;
}   

int prompt(void)
{
	char ch;
    fflush(stdin);
	printf("Perbaharui? (Tekan Y untuk update, tekan selain itu untuk tidak)");
	ch = getchar();
	ch = toupper(ch);
	fflush(stdin);
	if(ch == 'Y')
	{
		printf("Masukkan data baru: ");
		return(1);
	}
	else
	return(0);
}
