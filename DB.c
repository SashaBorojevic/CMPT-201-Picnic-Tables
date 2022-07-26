
/*------------------------------------------------------------
Student's Name: Sasa Borojevic
CMPUT 201 Spring 2022
Assignment 2
Lab Section: X01L
Lab Instructor's Name: Alex Krieger
*-----------------------------------------------------------*/


#include "DB.h"
// During development, enable pause() operation with #define PAUSE:
char pause(char *prompt) {
#ifdef PAUSE
	printf("%s Press ENTER ...", prompt);
	return getchar();
#endif
}

DataBase* importDB(char *fileName) {
    int num_lines;
    char line[MAX_LEN];
    /* Allocating memory to each structure of the database*/
    DataBase *Db = (DataBase *) malloc (sizeof(DataBase));
    Db->tableTypeTable = malloc (50 * sizeof(Table));
    Db->surfaceMaterialTable = malloc (50* sizeof(Table));
    Db->structuralMaterialTable = malloc (50* sizeof(Table));
    Db->neighbourhoodTable = malloc (MAX_LEN* sizeof(NeighbourhoodTable));
    Db->ttypeCount = Db->surfCount = Db->structCount = Db->neighCount = Db->neighIdCount = 0;
    num_lines = count_lines(fileName);
    Db->picnicTableSize = num_lines;
    Db->picnicTableTable = malloc ((num_lines)*sizeof(PicnicTable));

    /* Reading the file and storing the values into structures. */
    FILE *f = fopen (fileName, "r");
    /* Check if the file can be open. */
    if (f == NULL) {printf("Couldn't open the file from importDB");}
    /* Reading each line of the file and parsing with scan_line. */
    int row_count = 0;
    while (fgets(line, sizeof(line), f)) {
        /* skip the 1st line */
        if (row_count != 0){
            /*parse each line in scan_line and populate tables*/
            scan_line(Db, line, row_count-1);
        }
        row_count++;/* increase row count */
    }
    fclose(f);
    /* print tables */
    showTableTypes(Db);
    showSurfaceMaterials(Db);
    showStructuralMaterials(Db);
    showNeighbourhoodTable(Db);
return Db;
}

/* Parses id-th data line from csv file, and enters the extracted values
into the DataBase *Db. */
void scan_line(DataBase *Db, char *line, int i) {
    char* token;
    int token_count = 0, len_location;
    token = strtok(line, ",");
    /* Adding tableId to picnic table  */
    Db->picnicTableTable[i].tableId = i;
    /* Going trough tokens and parsing the line */
    while (token != NULL){
        /* Inserting values in to the picnic table. */
        if (token_count == 0) {
            Db->picnicTableTable[i].siteId = atoi(token);
        }
        /* Inserting values in to the tableTypeTable and picnic table. */
        if (token_count == 1) {
            if (check_table(Db->tableTypeTable,Db->ttypeCount, token)!= 1) {
                Db->tableTypeTable[Db->ttypeCount].name = malloc ((strlen(token)+1)*sizeof(char));
                strcpy(Db->tableTypeTable[Db->ttypeCount].name, token);
                Db->tableTypeTable[Db->ttypeCount].id = Db->ttypeCount;
                Db->ttypeCount++;
            }
            /* Inserting tabletypeId into picnic table */
            Db->picnicTableTable[i].tableTypeId = get_id(Db->tableTypeTable, token, Db->ttypeCount);
        }
        /* Inserting values in to surfaceMaterialTable */
        if (token_count == 2) {
            if (check_table(Db->surfaceMaterialTable,Db->surfCount, token) != 1) {
                Db->surfaceMaterialTable[Db->surfCount].name = malloc ((strlen(token)+1)*sizeof(char));
                strcpy(Db->surfaceMaterialTable[Db->surfCount].name, token);
                Db->surfaceMaterialTable[Db->surfCount].id = Db->surfCount;
                Db->surfCount++;
            }
            /* Inserting sufaceId into picnic table */
            Db->picnicTableTable[i].surfaceMaterialId = get_id(Db->surfaceMaterialTable, token, Db->surfCount);
        }
        /* Inserting values in to structuralMaterialTable and picnic table */
        if (token_count == 3) {
            if (check_table(Db->structuralMaterialTable,Db->structCount, token) != 1) {
                Db->structuralMaterialTable[Db->structCount].name = malloc ((strlen(token)+1)*sizeof(char));
                strcpy(Db->structuralMaterialTable[Db->structCount].name, token);
                Db->structuralMaterialTable[Db->structCount].id = Db->structCount;
                Db->structCount++;
            }
            /* Inserting structMaterialId into picnic table */
            Db->picnicTableTable[i].structuralMaterialId = get_id(Db->structuralMaterialTable, token, Db->structCount);
        }
        /* Inserting street/avenue in to picnic table */
        if (token_count == 4) {
            Db->picnicTableTable[i].streetAvenue = malloc ((strlen(token)+1)*sizeof(char));
            strcpy(Db->picnicTableTable[i].streetAvenue, token);
        }
        /* Inserting ids in to neighbourhoodTable and picinc table*/
        if (token_count == 5) {
            if (checkNeighId_table(Db->neighbourhoodTable, Db->neighIdCount, token) != 1){
                Db->neighbourhoodTable[Db->neighIdCount].id = atoi(token);
                Db->neighIdCount++;
            }
            Db->picnicTableTable[i].neighbhdId = atoi(token);
        }
        /* Inserting neighbourhoods in to neighbourhoodTable */
        if (token_count == 6) {
            if (checkNeigh_table(Db->neighbourhoodTable, Db->neighCount, token) != 1){
                Db->neighbourhoodTable[Db->neighCount].name = malloc ((strlen(token)+1)*sizeof(char));
                strcpy(Db->neighbourhoodTable[Db->neighCount].name, token);
                Db->neighCount++;
            }
        }
        if (token_count == 7) {
            Db->picnicTableTable[i].ward = malloc ((strlen(token)+1)*sizeof(char));
            strcpy(Db->picnicTableTable[i].ward, token);
        }
        if (token_count == 8) {
            Db->picnicTableTable[i].latitude = malloc ((strlen(token)+1)*sizeof(char));
            strcpy(Db->picnicTableTable[i].latitude, token);
        }
        if (token_count == 9) {
            Db->picnicTableTable[i].longitude = malloc ((strlen(token)+1)*sizeof(char));
            strcpy(Db->picnicTableTable[i].longitude, token);
        }
        if (token_count == 10) {
            len_location = strlen(token);
            Db->picnicTableTable[i].location = malloc ((strlen(token)+1)*sizeof(char));
            strcpy(Db->picnicTableTable[i].location, token);
        }
        if (token_count == 11) {
            strcat(Db->picnicTableTable[i].location, ",");
            Db->picnicTableTable[i].location = realloc (Db->picnicTableTable[i].location, (len_location + strlen(token)+1)*sizeof(char));
            strcat(Db->picnicTableTable[i].location, token);
        }
        if (token_count == 12) {
            Db->picnicTableTable[i].geometryPoint = malloc ((strlen(token)+1)*sizeof(char));
            strcpy(Db->picnicTableTable[i].geometryPoint, token);
        }
    /* getting next token */
    token = strtok(NULL, ",");
    token_count++;
    }
}

/* Creates .csv file containing the information of the Database.
 * NOTE: the exported .csv file must be exactly the same as the original
 * .csv file from which the Database was created if none of editTableEntry
 * or editTable were called.
 */
void exportDB(DataBase *Db, char *fileName ){
    int i;
    FILE *f = fopen (fileName, "wb");
    /* Check if the file can be open. */
    if (f == NULL) {printf("Couldn't open the file from exportDB");}
    fprintf(f,"table id, site Id, Table Type Id, Surface Material Id, Structural Material Id, Street / Avenue, Neighbourhood Id, Ward, Latitude, Longitude, Location\n");
    /* inserting each row from picnic table */
    for (i = 0; i < Db->picnicTableSize ; i++) {
        fprintf(f,"%d, %d, %d, %d, %d, %s, %d, %s, %s, %s, %s\n", Db->picnicTableTable[i].tableId, Db->picnicTableTable[i].siteId,
                Db->picnicTableTable[i].tableTypeId, Db->picnicTableTable[i].surfaceMaterialId, Db->picnicTableTable[i].structuralMaterialId,
                Db->picnicTableTable[i].streetAvenue, Db->picnicTableTable[i].neighbhdId, Db->picnicTableTable[i].ward, Db->picnicTableTable[i].latitude,
                Db->picnicTableTable[i].longitude, Db->picnicTableTable[i].location);
    }
    fclose(f);
    printf("\nDatabase has been exported to file %s!!!\n", fileName);
}

/* Take the Database pointer, the name of a member of the picnicTable entry
 * and a value for that member as parameters and returns the number of entries
 * in the picnicTable which have that value in the memberName.
 *   memberName is one of: Structural Material, Surface Material, Table Type
 */
int countEntries(DataBase *Db, char *memberName, char *value){
    int counter = 0, id, i;

    if (strcmp(memberName, "Table Type") == 0) {
        id = get_id(Db->tableTypeTable, value, Db->ttypeCount);
        for (i = 0; i < Db->picnicTableSize; i++){
            if (Db->picnicTableTable[i].tableTypeId == id){
                counter++;
            }
        }
    }
    if (strcmp(memberName, "Structural Material") == 0) {
        id = get_id(Db->structuralMaterialTable, value, Db->structCount);
        for (i = 0; i < Db->picnicTableSize; i++){
            if (Db->picnicTableTable[i].structuralMaterialId == id){
                counter++;
            }
        }
    }
    if (strcmp(memberName, "Surface Material") == 0) {
        id = get_id(Db->surfaceMaterialTable, value, Db->surfCount);
        for (i = 0; i < Db->picnicTableSize; i++){
            if (Db->picnicTableTable[i].surfaceMaterialId == id){
                counter++;
            }
        }
    }
return counter;
}

/* compare function for picnic table neighborhood id */
int compare_neighId(const void * a, const void * b) {

    PicnicTable *A = (PicnicTable *)a;
    PicnicTable *B = (PicnicTable *)b;

return ( A->neighbhdId - B->neighbhdId );
}

/* compare function for picnic table ward */
int compare_ward(const void * a, const void * b) {

    PicnicTable *A = (PicnicTable *)a;
    PicnicTable *B = (PicnicTable *)b;

    if ((strcmp(A->ward, B->ward)) != 0)
        return (strcmp(A->ward, B->ward));
    return strcmp(A->ward, B->ward);
}

/* Take Database pointer and the name of a member of the picnicTable entry
 * as an argument and sort the table in ascending order of the entry values
 * of that member;  memberName is one of: Neighbourhood Id, Ward
 */
void sortByMember(DataBase *Db, char *memberName){

    if (strcmp(memberName, "Neighbourhood Id") == 0) {
        qsort(Db->picnicTableTable, Db->picnicTableSize, sizeof(PicnicTable), compare_neighId);
    }
    if (strcmp(memberName, "Ward") == 0) {
        qsort(Db->picnicTableTable, Db->picnicTableSize, sizeof(PicnicTable), compare_ward);
    }
    printf("\nDatabase has been sorted by %s!\n", memberName);
}

/* Take the Database pointer and, a tableID, the name of a member of the
 * picnicTable entry and a value for that member as parameters, and find the
 * entry which has that tableID and change its memberName value to newValue.
 *  memberName is one of: Structural Material, Surface Material, Table Type
 */
void editTableEntry(DataBase *Db, int tableID, char *memberName, char *value){
    int i, id;
    if (strcmp(memberName, "Table Type") == 0) {
        id = get_id(Db->tableTypeTable, value, Db->ttypeCount);
        for (i = 0; i < Db->picnicTableSize; i++){
            if (Db->picnicTableTable[i].tableId == tableID){
                Db->picnicTableTable[i].tableTypeId = id;
            }
        }
    }
    if (strcmp(memberName, "Structural Material") == 0) {
        id = get_id(Db->structuralMaterialTable, value, Db->structCount);
        for (i = 0; i < Db->picnicTableSize; i++){
            if (Db->picnicTableTable[i].tableId == tableID){
                Db->picnicTableTable[i].structuralMaterialId = id;
            }
        }
    }
    if (strcmp(memberName, "Surface Material") == 0) {
        id = get_id(Db->surfaceMaterialTable, value, Db->surfCount);
        for (i = 0; i < Db->picnicTableSize; i++){
            if (Db->picnicTableTable[i].tableId == tableID){
                Db->picnicTableTable[i].surfaceMaterialId = id;
            }
        }
    }
    printf("\n%s of entry no.%d has been changed to %d.", memberName, tableID, id);
}

/* Add a PicnicTable to  picnicTableTable as a new last entry
   Params:
	site is siteId
	t_type is table type code
	surf_type is surface material code
	stru_type is structural material code
	st_av is street/avenue
	neighId is neighbourhood id
	w is ward
	lat is latitude
	longt is longitude
	loc is location

    Pre: if t_type, surf_type, stru_type, neighId do not exist in their tables,
         this returns silently without modifying picnicTableTable

    Side Effects: picnicTableTable grown by one entry
*/
void addTable(DataBase *Db, int site, int t_type, int surf_type, int stru_type,
char *st_av, int neighId, char *w, char *lat, char *longt, char *loc){

    Db->picnicTableTable[Db->picnicTableSize].tableId = Db->picnicTableSize;

    Db->picnicTableTable[Db->picnicTableSize].siteId = site;

    Db->picnicTableTable[Db->picnicTableSize].tableTypeId = t_type;

    Db->picnicTableTable[Db->picnicTableSize].surfaceMaterialId = surf_type;

    Db->picnicTableTable[Db->picnicTableSize].structuralMaterialId = stru_type;

    Db->picnicTableTable[Db->picnicTableSize].streetAvenue = malloc ((strlen(st_av)+1)*sizeof(char));
    strcpy(Db->picnicTableTable[Db->picnicTableSize].streetAvenue, st_av);

    Db->picnicTableTable[Db->picnicTableSize].neighbhdId = neighId;

    Db->picnicTableTable[Db->picnicTableSize].ward = malloc ((strlen(w)+1)*sizeof(char));
    strcpy(Db->picnicTableTable[Db->picnicTableSize].ward, w);

    Db->picnicTableTable[Db->picnicTableSize].latitude = malloc ((strlen(lat)+1)*sizeof(char));
    strcpy(Db->picnicTableTable[Db->picnicTableSize].latitude, lat);

    Db->picnicTableTable[Db->picnicTableSize].longitude = malloc ((strlen(longt)+1)*sizeof(char));
    strcpy(Db->picnicTableTable[Db->picnicTableSize].longitude, longt);

    Db->picnicTableTable[Db->picnicTableSize].location = malloc ((strlen(loc)+1)*sizeof(char));
    strcpy(Db->picnicTableTable[Db->picnicTableSize].location, loc);

    Db->picnicTableSize++;
}
/* finds the name in the table that matches the id */
char* find_table_name (Table* table, int n, int id) {
    int i;
    for ( i = 0 ; i < n ; ++i) {
        if (table[i].id == id){
            return table[i].name;
        }
    }
}

/* print listing of picnic tables grouped by wards in ascending order. */
void reportByWard(DataBase *Db) {
    int i;
    qsort(Db->picnicTableTable, Db->picnicTableSize, sizeof(PicnicTable), compare_ward);
    for (i = 0; i < Db->picnicTableSize; i++){
        if (Db->picnicTableTable[i-1].ward && strcmp(Db->picnicTableTable[i].ward,Db->picnicTableTable[i-1].ward) == 0) {
            printf("%d %s %s %s %s %d %s %s %s\n", Db->picnicTableTable[i].siteId,
            find_table_name(Db->tableTypeTable, Db->ttypeCount, Db->picnicTableTable[i].tableTypeId),
            find_table_name(Db->surfaceMaterialTable, Db->surfCount, Db->picnicTableTable[i].surfaceMaterialId),
            find_table_name(Db->structuralMaterialTable, Db->structCount, Db->picnicTableTable[i].structuralMaterialId),
            Db->picnicTableTable[i].streetAvenue, Db->picnicTableTable[i].neighbhdId, Db->picnicTableTable[i].latitude,
            Db->picnicTableTable[i].longitude, Db->picnicTableTable[i].location);
        }
        else {
            printf("\n%s\n%d %s %s %s %s %d %s %s %s\n", Db->picnicTableTable[i].ward, Db->picnicTableTable[i].siteId,
            find_table_name(Db->tableTypeTable, Db->ttypeCount, Db->picnicTableTable[i].tableTypeId),
            find_table_name(Db->surfaceMaterialTable, Db->surfCount, Db->picnicTableTable[i].surfaceMaterialId),
            find_table_name(Db->structuralMaterialTable, Db->structCount, Db->picnicTableTable[i].structuralMaterialId),
            Db->picnicTableTable[i].streetAvenue, Db->picnicTableTable[i].neighbhdId, Db->picnicTableTable[i].latitude,
            Db->picnicTableTable[i].longitude, Db->picnicTableTable[i].location);

        }
    }
}

/* finds the name in the table that matches the id */
char* find_neighbourhood(NeighbourhoodTable* table, int n, int id) {
    int i;
    for ( i = 0 ; i < n ; ++i) {
        if (table[i].id == id){
            return table[i].name;
        }
    }
}
/* print listing of picnic tables grouped by neigbourhoods in ascending
 * alphabetical order.
 */
void reportByNeighbourhood(DataBase *Db) {
    int i;
    qsort(Db->picnicTableTable, Db->picnicTableSize, sizeof(PicnicTable), compare_neighId);
    for (i = 0; i < Db->picnicTableSize; i++){
        if (Db->picnicTableTable[i-1].neighbhdId && Db->picnicTableTable[i].neighbhdId == Db->picnicTableTable[i-1].neighbhdId) {
            printf("%d %s %s %s %s %s %s %s %s\n", Db->picnicTableTable[i].siteId,
            find_table_name(Db->tableTypeTable, Db->ttypeCount, Db->picnicTableTable[i].tableTypeId),
            find_table_name(Db->surfaceMaterialTable, Db->surfCount, Db->picnicTableTable[i].surfaceMaterialId),
            find_table_name(Db->structuralMaterialTable, Db->structCount, Db->picnicTableTable[i].structuralMaterialId),
            Db->picnicTableTable[i].streetAvenue, Db->picnicTableTable[i].ward, Db->picnicTableTable[i].latitude,
            Db->picnicTableTable[i].longitude, Db->picnicTableTable[i].location);
        }
        else {
            printf("\n%d:%s\n%d %s %s %s %s %s %s %s %s\n", Db->picnicTableTable[i].neighbhdId,
            find_neighbourhood(Db->neighbourhoodTable,Db->neighIdCount, Db->picnicTableTable[i].neighbhdId),
            Db->picnicTableTable[i].siteId,
            find_table_name(Db->tableTypeTable, Db->ttypeCount, Db->picnicTableTable[i].tableTypeId),
            find_table_name(Db->surfaceMaterialTable, Db->surfCount, Db->picnicTableTable[i].surfaceMaterialId),
            find_table_name(Db->structuralMaterialTable, Db->structCount, Db->picnicTableTable[i].structuralMaterialId),
            Db->picnicTableTable[i].streetAvenue, Db->picnicTableTable[i].ward, Db->picnicTableTable[i].latitude,
            Db->picnicTableTable[i].longitude, Db->picnicTableTable[i].location);

        }
    }
}

/* free all dynamically allocated memory */
void destroyDB (DataBase *Db) {
    int i, j, k, z, x;

    for (i = 0; i < Db->ttypeCount; i++) {
        free(Db->tableTypeTable[i].name);
        Db->tableTypeTable[i].name = NULL;
        Db->tableTypeTable[i].id = NULL;
    }
    free(Db->tableTypeTable);
    for (j = 0; j < Db->surfCount; j++) {
        free(Db->surfaceMaterialTable[j].name);
        Db->surfaceMaterialTable[j].name = NULL;
        Db->surfaceMaterialTable[j].id = NULL;
    }
    free(Db->surfaceMaterialTable);
    for (k = 0; k < Db->structCount; k++) {
        free(Db->structuralMaterialTable[k].name);
        Db->structuralMaterialTable[k].name = NULL;
        Db->structuralMaterialTable[k].id = NULL;
    }
    free(Db->structuralMaterialTable);

    for (z = 0; z < Db->neighCount; z++){
        free(Db->neighbourhoodTable[z].name);
        Db->neighbourhoodTable[z].name = NULL;
        Db->neighbourhoodTable[z].id = NULL;
    }
    free(Db->neighbourhoodTable);
    for (x = 0; x < Db->picnicTableSize; x++){
        free(Db->picnicTableTable[x].streetAvenue);
        Db->picnicTableTable[x].streetAvenue = NULL;
        free(Db->picnicTableTable[x].ward);
        Db->picnicTableTable[x].ward = NULL;
        free(Db->picnicTableTable[x].longitude);
        Db->picnicTableTable[x].longitude = NULL;
        free(Db->picnicTableTable[x].latitude);
        Db->picnicTableTable[x].latitude = NULL;
        free(Db->picnicTableTable[x].location);
        Db->picnicTableTable[x].location = NULL;
        free(Db->picnicTableTable[x].geometryPoint);
        Db->picnicTableTable[x].geometryPoint = NULL;
        Db->picnicTableTable[x].neighbhdId = NULL;
        Db->picnicTableTable[x].siteId = NULL;
        Db->picnicTableTable[x].structuralMaterialId = NULL;
        Db->picnicTableTable[x].surfaceMaterialId = NULL;
        Db->picnicTableTable[x].tableId = NULL;
        Db->picnicTableTable[x].tableTypeId = NULL;
    }
    free(Db->picnicTableTable);
    Db->picnicTableSize = 0;
    Db->ttypeCount = Db->surfCount = Db->structCount = Db->neighCount = Db->neighIdCount = 0;
    free(Db);
    printf("\nDatabase has been destroyed!!!\n");
}


/* gets the correct id for the requested name */
int  get_id(Table *table, char *token, int n) {
    int i;
    for ( i = 0 ; i < n ; ++i) {
        if (table[i].name && strcmp(table[i].name, token) == 0){
            return table[i].id;
        }
    }
return 0;
}

/* function prints tableTypeTable ids and names */
void showTableTypes(DataBase* Db) {
	int i;
	printf("\ntableTypeTable\n\n      Id type\n");
	for (int i = 0; i < Db->ttypeCount ; i++) {
        printf("      %1d  %s\n", Db->tableTypeTable[i].id, Db->tableTypeTable[i].name);
	}
	pause("");
}

/* function prints surfaceMaterialTable ids and names */
void showSurfaceMaterials(DataBase* Db) {
	int i;
	printf("\nsurfaceMaterialTable\n\n      Id surface\n");
    for (int i = 0; i < Db->surfCount ; i++) {
        printf("      %1d  %s\n", Db->surfaceMaterialTable[i].id, Db->surfaceMaterialTable[i].name);
    }
    pause("");
}

/* function prints structuralMaterialTable ids and names */
void showStructuralMaterials(DataBase* Db) {
    int i;
	printf("\nstructuralMaterialTable\n\n      Id structure\n");
    for (int i = 0; i < Db->structCount ; i++) {
        printf("      %1d  %s\n", Db->structuralMaterialTable[i].id, Db->structuralMaterialTable[i].name);
    }
    pause("");
}

/* function prints neighbourhoodTable ids and names */
void showNeighbourhoodTable(DataBase* Db) {
    int i;
	printf("\nneighbourhoodTable\n\n      Id      name\n");
    for (int i = 0; i < Db->neighCount ; i++) {
        printf("      %2d    %s\n", Db->neighbourhoodTable[i].id, Db->neighbourhoodTable[i].name);
    }
    pause("");
}

/* function checks if a name exists in the table*/
int check_table(Table* table, int n, char* token) {
    int i;
    for ( i = 0 ; i < n ; ++i) {
        if (table[i].name && strcmp(table[i].name, token) == 0){
            return 1;
        }
    }
return 0;
}

/* function checks if a neighbourhood exists in the table*/
int checkNeigh_table(NeighbourhoodTable* table, int n, char* token) {
    int i;
    for ( i = 0 ; i < n ; ++i) {
        if (table[i].name && strcmp(table[i].name, token) == 0){
            return 1;
        }
    }
return 0;
}

/* function checks if a neighbourhood ID exists in the table*/
int checkNeighId_table(NeighbourhoodTable* table, int n, char* token) {
    int i;
    for ( i = 0 ; i < n ; ++i) {
        if (table[i].id && table[i].id == atoi(token)){
            return 1;
        }
    }
return 0;
}
/* counts the lines in the file including the header*/
int count_lines(char *filename){
    char ch;
    int linesCount=0;
    //open file in read more
    FILE *fp = fopen(filename,"r");
    if(fp == NULL) {
        printf("File \"%s\" does not exist from count_lines!!!\n",filename);
        return -1;
    }
    //read character by character and check for new line
    while((ch=fgetc(fp))!=EOF) {
        if(ch == '\n')
            linesCount++;
    }
    //close the file
    fclose(fp);

return linesCount;
}

void safeReadString(char *str,int size) {
    char c;
    char* s=str;
    int count=0;
    c=getchar();
    /* changed '\0' to '\n' */
    while((c!='\n') && (count<size)) {
        *s = c;
        s++;
        count++;
        c=getchar();
    }
    *s = '\0';
}

/* main driver program*/
int main() {
    char* fileName = malloc (100*sizeof(char));
    char* exportfileName = malloc (100*sizeof(char));
    char* memberName = malloc (100*sizeof(char));
    char* value = malloc (100*sizeof(char));
    DataBase *data;
    int choice, tableID;

    do {
        printf("\nMenu\n");
        printf("1. Import Database\n");
        printf("2. Export Database\n");
        printf("3. Count Entries\n");
        printf("4. Sort By Member\n");
        printf("5. Edit Table Entry\n");
        printf("6. Add an Entry to the Picnic Table\n");
        printf("7. Report By Ward\n");
        printf("8. Report By Neighbourhood\n");
        printf("9. Destroy Database\n");
        printf("10. Exit\n");
        printf("Choose a number from the menu: ");
        scanf("%d",&choice);

        switch (choice) {
            case 1:
                printf("Import Database Chosen!\n");
                printf("Enter a file name for import: ");
                getchar();
                scanf("%s",fileName);
                data = importDB(fileName);
                break;
            case 2:
                printf("Export Database Chosen!\n");
                printf("Enter the file name you would like the database to be exported to: ");
                getchar();
                scanf("%s",exportfileName);
                exportDB(data, exportfileName);
                break;
            case 3:
                printf("Count Entries Chosen!");
                printf("\nEnter a member name: ");
                getchar();
                scanf("%[^\n]%*c",memberName);
                printf("Enter a value: ");
                scanf("%[^\n]%*c",value);
                printf("\nThe count of %s in %s is %d\n", value, memberName, countEntries(data, memberName, value));
                break;
            case 4:
                printf("Sort by member chosen!");
                printf("\nEnter a member name: ");
                getchar();
                scanf("%[^\n]%*c",memberName);
                sortByMember(data, memberName);
                break;
            case 5:
                printf("Edit table entry chosen!");
                printf("\nEnter a table id: ");
                scanf("%d", &tableID);
                getchar();
                printf("Enter a member name: ");
                scanf("%[^\n]%*c", memberName);
                printf("Enter a value: ");
                scanf("%[^\n]%*c", value);
                editTableEntry(data, tableID, memberName, value);
                break;
            case 6: /*Call function here to do the required operation*/
                break;
            case 7:
                printf("Report by ward chosen!\n");
                reportByWard(data);
                break;
            case 8:
                printf("Report by neighbourhood chosen!\n");
                reportByNeighbourhood(data);
                break;
            case 9:
                printf("Destroy database chosen!\n");
                destroyDB(data);
                break;
            case
            10: printf("\nGoodbye!!\n");
                break;
            default: printf("Wrong Choice. Enter again!\n");
                break;
        }

    }while (choice != 10);
return 0;
}
