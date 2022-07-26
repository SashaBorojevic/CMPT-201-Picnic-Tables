/******************************************************************************

DB.h  220518 - Start with this.

.csv file ends line with /r/n

Picnic Table .csv file structure:
Top line: Id,Table Type,Surface Material,Structural Material,Street / Avenue,
          Neighbourhood Id,Neighbourhood Name,Ward,Latitude,Longitude,Location,
          Geometry Point
followed by lines like:
        34500,Square Picnic Table,Wood,Aggregate,WILLIAM HAWRELAK PARK ROAD NW,
		5370,RIVER VALLEY MAYFAIR,Ward papastew,53.52973241943662,
		-113.55438728932153,"(53.52973241943662, -113.55438728932153)",
		POINT (-113.55438728932153 53.52973241943662)

Thus, data from each line would be stored in a PicnicTable struct-ure, like:
Id 						ex. 34500
Table Type 				ex. Square Picnic Table
Surface Material		ex. Wood
Structural Material		ex. Aggregate
Street / Avenue 		ex. WILLIAM HAWRELAK PARK ROAD NW
Neighbourhood Id 		ex. 5370
Neighbourhood Name 		ex. RIVER VALLEY MAYFAIR
Ward 					ex. "Ward papastew"
Latitude				ex. 53.52973241943662
Longitude 				ex. -113.55438728932153
Location 				ex. "(53.52973241943662, -113.55438728932153)"
Geometry Point 			ex. "POINT (-113.55438728932153 53.52973241943662)"
******************************************************************************/

#ifndef DB.H
#define DB.H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 300
#define NUM_TOKENS 12

typedef struct {
    int id;
    char *name;
} Table;

typedef struct {
    int id;
    char *name;

} NeighbourhoodTable;


typedef struct {
    int tableId, siteId, tableTypeId, surfaceMaterialId, structuralMaterialId;
    char *streetAvenue;
    int  neighbhdId;
    char *ward, *latitude, *longitude, *location, *geometryPoint;

} PicnicTable;

typedef struct {
    Table              *tableTypeTable;
    Table              *surfaceMaterialTable;
    Table              *structuralMaterialTable;
    NeighbourhoodTable *neighbourhoodTable;
    PicnicTable        *picnicTableTable;
	int picnicTableSize;// num of lines in file-1
    int neighCount;
    int structCount;
    int surfCount;
    int ttypeCount;
    int neighIdCount;

} DataBase;

/* DataBase* importDB(char *fileName)
   Creates and populates Database with the corresponding data set information;
   and returns Database pointer:
*-->{							// below pointers to malloc for:
      Table *tableTypeTable;					// Table * 50
      Table *surfaceMaterialTable;				// Table * 50
      Table *structuralMaterialTable;			// Table * 50
      NeighbourhoodTable *neighbourhoodTable;	// NeighbourhoodTable * 300
      PicnicTable *picnicTableTable;			// PicnicTable* lines in file-1
      int picnicTableSize;						// num of lines in file-1
      int neighCount;							//
      int structCount;							//
      int surfCount;							//
      int ttypeCount; }							//


Procedure:
 - Db *-> malloc for one struct DataBase; pointer Db returned
 - tableTypeTable *-> malloc for 50 struct Table
 - surfaceMaterialTable *-> malloc for 50 struct Table
 - structuralMaterialTable *-> malloc for 50 struct Table
 - neighbourhoodTable *-> malloc for 300 struct NeighbourhoodTable
 - set neighCount = structCount = surfCount = ttypeCount = 0
 - set picnicTableSize = number of lines in file - 1
 - picnicTableTable *-> malloc for PicnicTable * (number of lines in file - 1)
 - skip first line in file
 - for each data line, starting at i = 0, read in line, and extract all items
   into respective fields in DataBase:
    - PicnicTable.tableId = file line number (starts at 0, ++ for every line)
    -
 */
DataBase*  importDB(char *fileName);

/* Creates .csv file containing the information of the Database.
 * NOTE: the exported .csv file must be exactly the same as the original
 * .csv file from which the Database was created if none of editTableEntry
 * or editTable were called.
 */
void exportDB(DataBase *Db, char *fileName );

/* Take the Database pointer, the name of a member of the picnicTable entry
 * and a value for that member as parameters and returns the number of entries
 * in the picnicTable which have that value in the memberName.
 *   memberName is one of: Structural Material, Surface Material, Table Type
 */
int countEntries(DataBase *Db, char *memberName, char *value);

/* Take Database pointer and the name of a member of the picnicTable entry
 * as an argument and sort the table in ascending order of the entry values
 * of that member;  memberName is one of: Neighbourhood Id, Ward
 */
void sortByMember(DataBase *Db, char *memberName);

/* Take the Database pointer and, a tableID, the name of a member of the
 * picnicTable entry and a value for that member as parameters, and find the
 * entry which has that tableID and change its memberName value to newValue.
 *  memberName is one of: Structural Material, Surface Material, Table Type
 */
void editTableEntry(DataBase *Db, int tableID, char *memberName, char *value);

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
              char *st_av, int neighId, char *w, char *lat, char *longt,
			  char *loc);

/* print listing of picnic tables grouped by wards in ascending order.
*/
void reportByWard(DataBase *Db);

/* print listing of picnic tables grouped by neigbourhoods in ascending
 * alphabetical order.
 */
void reportByNeighbourhood(DataBase *Db);

/* free all dynamically allocated memory
 */
void destroyDB (DataBase *Db);

//-----------------------------------------------------------------------------
// USEFUL SUPPORT FUNCTIONS

/*-----------------------------------------------------------------------------
void scan_line(DataBase* Db, char *line, int id)
Parses id-th data line from csv file, and enters the extracted values
into the DataBase *Db.
Recall format of top and data lines, split up below for legibility:

Id,Table Type,Surface Material,Structural Material,Street / Avenue,
34500,Square Picnic Table,Wood,Aggregate,WILLIAM HAWRELAK PARK ROAD NW,

Neighbourhood Id,Neighbourhood Name,Ward,Latitude,Longitude,
5370,RIVER VALLEY MAYFAIR,Ward papastew,53.52973241943662,-113.55438728932153,

Location,
"(53.52973241943662, -113.55438728932153)",

Geometry Point
POINT (-113.55438728932153 53.52973241943662)

Alternately:
Field                  Ex. item (or token)             Store to:
Id                     34500                           PicnicTable.siteId
Table Type             Square Picnic Table
Surface Material       Wood
Structural Material    Aggregate
Street / Avenue        WILLIAM HAWRELAK PARK ROAD NW
Neighbourhood Id       5370
Neighbourhood Name     RIVER VALLEY MAYFAIR
Ward                   Ward papastew
Latitude               53.52973241943662
Longitude              -113.55438728932153
Location               "(53.52973241943662,
                         -113.55438728932153)"
Geometry Point         POINT (-113.55438728932153
                                53.52973241943662)

PicnicTable:  { int  tableId,                          file line number
                     siteId,
					 tableTypeId,
                     surfaceMaterialId,
					 structuralMaterialId;
                char *streetAvenue;
                int  neighbhdId ;
                char *ward, *latitude, *longitude, *location; }

NOTE: if tokenizing (could also sscanf()): 12 tokens (i = 0 to 11); ex.:
0     1                   2    3         4
34500,Square Picnic Table,Wood,Aggregate,WILLIAM HAWRELAK PARK ROAD NW,
5    6                    7             8
5370,RIVER VALLEY MAYFAIR,Ward papastew,53.52973241943662,
9                   10
-113.55438728932153,"(53.52973241943662, -113.55438728932153)",
11
POINT (-113.55438728932153 53.52973241943662)
-----------------------------------------------------------------------------*/

/* Parses id-th data line from csv file, and enters the extracted values
into the DataBase *Db.
*/
void scan_line(DataBase *Db, char *line, int);

char *save_find_neigh(DataBase *Db, int n_num, char *n_name, int find);

int  find_table_id(DataBase *Db, char *t_type);

int  find_surf_id(DataBase *Db, char *surf_mat);

int  find_struct_id(DataBase *Db, char *struct_mat);

void write_entry(DataBase* Db, PicnicTable, FILE *);

char *get_geometry(char *);

int count_lines (char *file_name);

#endif /* DB_H */
