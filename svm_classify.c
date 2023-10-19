/***********************************************************************/
/*                                                                     */
/*   svm_classify.c                                                    */
/*                                                                     */
/*   Classification module of Support Vector Machine.                  */
/*                                                                     */
/*   Author: Thorsten Joachims                                         */
/*   Date: 02.07.02                                                    */
/*                                                                     */
/*   Copyright (c) 2002  Thorsten Joachims - All rights reserved       */
/*                                                                     */
/*   This software is available for non-commercial use only. It must   */
/*   not be modified and distributed without prior permission of the   */
/*   author. The author is not responsible for implications from the   */
/*   use of this software.                                             */
/*                                                                     */
/************************************************************************/

# include "svm_common.h"

char docfile[200];

char predictionsfile[200];

void read_input_parameters(int, char **, char *, char *, char *, long *, 
			   long *);
void print_help(void);


void run_SVM(MODEL *model, float feature1, float feature2, float feature3, float feature4, float feature5)
{
  DOC *doc;   /* test example */
  WORD *words;
  long max_docs,max_words_doc,lld;
  long totdoc=0,queryid,slackid;
  long correct=0,incorrect=0,no_accuracy=0;
  long res_a=0,res_b=0,res_c=0,res_d=0,wnum,pred_format;
  long j;
  double t1,runtime=0;
  double dist,doc_label,costfactor;
  char *line,*comment; 

  max_words_doc+=2;
  lld+=2;

  line = (char *)my_malloc(sizeof(char)*lld);
  words = (WORD *)my_malloc(sizeof(WORD)*(max_words_doc+10));
  
  if(model->kernel_parm.kernel_type == 0) { /* linear kernel */
    /* compute weight vector */
    add_weight_vector_to_linear_model(model);
  }

  {
    int offset = 0;
    offset += sprintf(line + offset, "%d ", 0);
    offset += sprintf(line + offset, "1:%f ", feature1);
    offset += sprintf(line + offset, "2:%f ", feature2);
    offset += sprintf(line + offset, "3:%f ", feature3);
    offset += sprintf(line + offset, "4:%f ", feature4);
    offset += sprintf(line + offset, "5:%f", feature5);
    line[offset] = '\0';
    printf("%s", line);
    
  }

    printf("\n%f\n", doc_label);
    parse_document(line,words,&doc_label,&queryid,&slackid,&costfactor,&wnum,
		   max_words_doc,&comment);
    totdoc++;
    if(model->kernel_parm.kernel_type == 0) {   /* linear kernel */
      for(j=0;(words[j]).wnum != 0;j++) {  /* Check if feature numbers   */
	if((words[j]).wnum>model->totwords) /* are not larger than in     */
	  (words[j]).wnum=0;               /* model. Remove feature if   */
      }                                        /* necessary.                 */
      doc = create_example(-1,0,0,0.0,create_svector(words,comment,1.0));
      t1=get_runtime();
      dist=classify_example_linear(model,doc);
      runtime+=(get_runtime()-t1);
      free_example(doc,1);
    }
    else {                             /* non-linear kernel */
      doc = create_example(-1,0,0,0.0,create_svector(words,comment,1.0));
      t1=get_runtime();
      dist=classify_example(model,doc);
      runtime+=(get_runtime()-t1);
      free_example(doc,1);
    }
    printf("\n%f", dist);
  free(line);
  free(words);
  free_model(model,1);
}

int main()
{
  char modelfile[200];
  sprintf(modelfile, "%s", "model");
  MODEL *model;
  model=read_model(modelfile);
  run_SVM(model, 8, 8, 11, 11, 1000.303);
  return 0;
}




