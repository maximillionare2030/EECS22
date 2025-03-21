/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* ImageList.c: source file for image list manipulations             */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "ImageList.h"
#include "Image.h"

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
        free(entry);
        entry = next;
    }

	/* to be implemented */
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, Image *RGBimage)
{
    assert(list);
    asset(RGBimage);

    IENTRY *entry = (IENTRY *)(malloc(sizeof(IENTRY)));
    entry -> List = list;
    entry -> Next = NULL;
    entry -> Prev = list -> Last;
    entry -> Image = RGBimage;

    list -> Last -> Next = entry;
    list -> Last = entry;

	/* to be implemented */
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVImage *YUVimage)
{
	/* to be implemented */
}

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
	/* to be implemented */
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
	/* to be implemented */
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
	/* to be implemented */
}

/* Insert a RGB image to the image list in the front */
void PrependRGBImage(ILIST *list, Image *RGBimage)
{
	/* to be implemented */
}


/* EOF */
