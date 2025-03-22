/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* ImageList.c: source file for image list manipulations             */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "ImageList.h"
#include "Image.h"

IENTRY *CreateImageEntry() {
    IENTRY *entry = (IENTRY *)malloc(sizeof(IENTRY));
    assert(entry);
    entry->Image = NULL;
    entry->Next = NULL;
    entry->Prev = NULL;
    entry->List = NULL;
    return entry;

}

void DeleteImageEntry(IENTRY *entry) {
    assert(entry);

    if (entry->Image) {
        // Use the appropriate deletion function
        if (entry->List && entry->List->First && entry->List->First->Image) {
            if (entry->List->First->Image == entry->Image) {
                // Assume it's an RGB image
                DeleteImage((Image *)entry->Image);
            } else {
                // Assume it's a YUV image
                DeleteYUVImage((YUVImage *)entry->Image);
            }
        }
    }

    free(entry);
}


/* Create a new image list */
ILIST *CreateImageList(void)
{
    ILIST *list;
    list = (ILIST *)(malloc(sizeof(ILIST)));

    assert(list);

    list -> Length = 0;
    list -> First = NULL;
    list -> Last = NULL;

    return list;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
    assert(list);

    IENTRY *entry = list -> First;
    while (entry != NULL) {
        IENTRY *next = entry -> Next;
        DeleteImageEntry(entry);
        entry = next;
    }

    free(list);

	/* to be implemented */
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, Image *RGBimage)
{
    assert(list);
    assert(RGBimage);

    IENTRY *entry = CreateImageEntry();
    entry -> Image = RGBimage;
    if (list -> Length == 0) {
        list -> First = entry;
        list -> Last = entry;
        list -> Length = 1;
    }
    else {
        entry -> List = list;
        entry -> Prev = list -> Last;
        list -> Last -> Next = entry;
        list -> Last = entry;
        list -> Length += 1;
    }
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVImage *YUVimage)
{
	assert(list);
    assert(YUVimage);

    IENTRY *entry = CreateImageEntry();
    entry -> Image = YUVimage;
    if (list -> Length == 0) {
        list -> First = entry;
        list -> Last = entry;
        list -> Length = 1;
    }
    else {
        entry -> List = list;
        entry -> Prev = list -> Last;
        list -> Last -> Next = entry;
        list -> Last = entry;
        list -> Length += 1;
    }
}


/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end) {
    if (!list || start >= end || end > list->Length) {
        return; // Invalid range or list
    }

    IENTRY *entry = list->First;
    unsigned int index = 0;

    // Delete entries before `start`
    while (index < start && entry != NULL) {
        IENTRY *next = entry->Next;
        DeleteImageEntry(entry);
        entry = next;
        list->Length--;
        index++;
    }

    // Update the First pointer
    list->First = entry;
    if (list->First) {
        list->First->Prev = NULL;
    }

    // Move to `end`
    while (index < end && entry != NULL) {
        entry = entry->Next;
        index++;
    }

    // Delete entries after `end`
    if (entry) {
        IENTRY *tail = entry->Next;
        while (tail != NULL) {
            IENTRY *next = tail->Next;
            DeleteImageEntry(tail);
            tail = next;
            list->Length--;
        }
        entry->Next = NULL; // Terminate the list at `end`
    }

    // Update the Last pointer
    list->Last = entry;
    if (list->Last) {
        list->Last->Next = NULL;
    }
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
    assert(list);
    if (factor <= 1 || list->Length == 0) {
        return; // Do nothing if factor is invalid
    }

    unsigned int count = 0;
    IENTRY *entry = list->First;
    IENTRY *prev = NULL;

    while (entry != NULL) {
        IENTRY *next = entry->Next;

        if (count % factor != 0) {
            // Remove the current entry
            if (prev) {
                prev->Next = next;
            } else {
                list->First = next; // Update head
            }

            if (next) {
                next->Prev = prev;
            } else {
                list->Last = prev; // Update tail
            }

            DeleteImageEntry(entry);
            list->Length--;
        } else {
            prev = entry; // Move prev pointer forward
        }

        entry = next;
        count++; // Tracker of ith frame
    }
}

//* Reverse an Image List */
void ReverseImageList(ILIST *list)
{
    assert(list);
    if (list->Length <= 1) {
        return; // No need to reverse if 0 or 1 element
    }

    IENTRY *current = list->First;
    IENTRY *temp = NULL;

    // Swap `Next` and `Prev` for each node
    while (current != NULL) {
        temp = current->Prev;
        current->Prev = current->Next;
        current->Next = temp;
        current = current->Prev; // Move to the next node (was previously `Next`)
    }

    // Swap first and last pointers
    temp = list->First;
    list->First = list->Last;
    list->Last = temp;
}


/* Insert a RGB image to the image list in the front */
void PrependRGBImage(ILIST *list, Image *RGBimage)
{
    assert(list);
    assert(RGBimage);

    IENTRY *entry = CreateImageEntry();
    entry->Image = RGBimage;
    entry->List = list;

    if (list->Length == 0) {
        list->First = entry;
        list->Last = entry;
    } else {
        entry->Next = list->First;
        list->First->Prev = entry;
        list->First = entry;
    }

    list->Length += 1;
}


/* EOF */
