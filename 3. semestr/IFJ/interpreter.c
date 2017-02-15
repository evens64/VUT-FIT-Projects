// Příklad:		Interpret jazyka IFJ16 [IFJ/IAL projekt]
// Autoři:		Jakub Hud (xhudja00)

#include "interpreter.h"

void interprete(tBST *BST){

	globalVarHandle(BST);

	BSTSetActiveTop(BST);

	tMember *MainClassMember = BSTSearchMember(BST, "Main");
	BST->Depth++;
	BST->Active = MainClassMember->NextLevel;

    tIdentifier *MainFceId = BSTSearchIdentifier(*BST,"run");

    String tempstr = NULL;

	TInstr *item = ((tLevel*)(MainFceId->value))->fceStart;
    TInstr *tempitem;
	InstrTypes instr;

	tIdentifier *fceVarTree;// pomocna promenna
	int i; // pomocna promenna

	while(item != NULL){

		instr = item->t;

		switch(instr){

			case ass_int:

			    #ifdef debug
                    printf("interpreter: assigning int %d\n",*((int*)(item->op1)));
                #endif

				*((int*)(item->res)) = *((int*)(item->op1));

				break;

			case eq_int:

                #ifdef debug
                    printf("interpreter: op: %d == %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				if(*((int*)(item->op1)) == *((int*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}


				break;

			case noteq_int:

                #ifdef debug
                    printf("interpreter: op: %d == %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				if(*((int*)(item->op1)) != *((int*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case gt_int:

                #ifdef debug
                    printf("interpreter: op: %d > %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				if(*((int*)(item->op1)) > *((int*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case ls_int:

                #ifdef debug
                    printf("interpreter: op: %d < %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				if(*((int*)(item->op1)) < *((int*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}


				break;

			case lseq_int:

                #ifdef debug
                    printf("interpreter: op: %d <= %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				if(*((int*)(item->op1)) <= *((int*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case gteq_int:

                #ifdef debug
                    printf("interpreter: op: %d >= %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				if(*((int*)(item->op1)) >= *((int*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case add_int:

                #ifdef debug
                    printf("interpreter: op: %d + %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				*((int*)(item->res)) = (*((int*)(item->op1))) + (*((int*)(item->op2)));

				break;

			case sub_int:

                #ifdef debug
                    printf("interpreter: op: %d - %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				*((int*)(item->res)) = (*((int*)(item->op1))) - (*((int*)(item->op2)));

				break;

			case div_int:

			    #ifdef debug
                    printf("interpreter: op: %d / %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

                if((*((double*)(item->op2))) == 0 ) print_error(err_run_div);
				*((int*)(item->res)) = (*((int*)(item->op1))) / (*((int*)(item->op2)));

				break;

			case mul_int:

			    #ifdef debug
                    printf("interpreter: op: %d * %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
                #endif

				*((int*)(item->res)) = (*((int*)(item->op1))) * (*((int*)(item->op2)));

				break;
           case  ass_dbl:

				*((double*)(item->res)) = *((double*)(item->op1));
				break;

				#ifdef debug
                    printf("interpreter: op: assigning double\n");
                #endif

			case eq_dbl:

                #ifdef debug
                    printf("interpreter: op: %f == %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				if(*((double*)(item->op1)) == *((double*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case noteq_dbl:

				#ifdef debug
                    printf("interpreter: op: %f != %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				if(*((double*)(item->op1)) != *((double*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case gt_dbl:

                #ifdef debug
                    printf("interpreter: op: %f > %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				if(*((double*)(item->op1)) > *((double*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}
				break;

			case ls_dbl:

				#ifdef debug
                    printf("interpreter: op: %f < %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				if(*((double*)(item->op1)) < *((double*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case lseq_dbl:

				#ifdef debug
                    printf("interpreter: op: %f <= %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				if(*((double*)(item->op1)) <= *((double*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case gteq_dbl:

				#ifdef debug
                    printf("interpreter: op: %f >= %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				if(*((double*)(item->op1)) >= *((double*)(item->op2))){

					*((unsigned char*)(item->res)) = 255;
				}
				else{
					*((unsigned char*)(item->res)) = 0;
				}

				break;

			case add_dbl:

                #ifdef debug
                    printf("interpreter: op: %f + %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				*((double*)(item->res)) = (*((double*)(item->op1))) + (*((double*)(item->op2)));

				break;

			case sub_dbl:

                #ifdef debug
                    printf("interpreter: op: %f - %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				*((double*)(item->res)) = (*((double*)(item->op1))) - (*((double*)(item->op2)));

				break;

			case div_dbl:

			    #ifdef debug
                    printf("interpreter: op: %f / %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

                if((*((double*)(item->op2))) == 0 ) print_error(err_run_div);
				*((double*)(item->res)) = (*((double*)(item->op1))) / (*((double*)(item->op2)));

				break;

			case mul_dbl:

                #ifdef debug
                    printf("interpreter: op: %f * %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
                #endif

				*((double*)(item->res)) = (*((double*)(item->op1))) * (*((double*)(item->op2)));

				break;

			case ass_str:

                #ifdef debug
                    printf("interpreter: op: assigning string %s\n", *((String*)(item->op1)));
                #endif

                i = 0;
				while( (*( (char**)(item->op1) ))[i] != 0 ){
					i++;
				}
                *(char**)item->res = realloc(*(char**)item->res,sizeof(char)*(i+1));
				if (*(char**)item->res == NULL) print_error(err_oth);
				for(; i >= 0; i--){
					( *((char**)(item->res)) )= ( *((char**)(item->op1)) );
				}

				break;

			case add_str:

                #ifdef debug
                    printf("interpreter: op: %s + %s\n", *((String*)(item->op1)), *((String*)(item->op2)));
                #endif
				tempstr = concat(*(char **)item->op1, *(char **)item->op2);
                if (*(String*)item->res != NULL) free(*(String*)item->res);
                *((String*)(item->res)) = tempstr;
				break;

			case ifj16readIntCall:

                #ifdef debug
                    printf("interpreter: call: ifj16readInt\n");
                #endif

				*((int*)(item->res)) = readInt();

				break;

			case ifj16readDoubleCall:

				#ifdef debug
                    printf("interpreter: call: ifj16readDouble\n");
                #endif

				*((double*)(item->res)) = readDouble();

				break;

			case ifj16readStringCall:

			    #ifdef debug
                    printf("interpreter: call: ifj16readString\n");
                #endif

                if (*(String*)item->res != NULL) free(*(String*)item->res);
				*((String*)(item->res)) = readString();

				break;

			case ifj16printCall:

				#ifdef debug
                    printf("interpreter: call: ifj16print\n");
                #endif

				print(
                (Ttoken*)(item->op2)
                );

				break;

			case ifj16lengthCall:

			    #ifdef debug
                    printf("interpreter: call: ifj16legth\n");
                #endif

				*((int*)(item->res)) = length( (String)((tParams_item*)item->op2)->Param );

				break;

			case ifj16substrCall:

				#ifdef debug
                    printf("interpreter: call: ifj16substr\n");
                #endif

				*((String*)(item->res)) = substr(
                    *((String*)(((tParams_item*)item->op2)->Param)),
                    *(int *)(((tParams_item*)item->op2)->Next->Param),
                    *(int *)(((tParams_item*)item->op2)->Next->Next->Param)
                );

				break;

			case ifj16compareCall:

                #ifdef debug
                    printf("interpreter: call: ifj16compare\n");
                #endif

				*((int*)(item->res)) = compare(
                    *((String*)(((tParams_item*)item->op2)->Param)),
                    *((String*)(((tParams_item*)item->op2)->Next->Param))
                );

				break;

			case ifj16findCall:

                #ifdef debug
                    printf("interpreter: call: ifj16find\n");
                #endif

				*((int*)(item->res)) = find(
                    *((String*)(((tParams_item*)item->op2)->Next->Param)),
                    *((String*)(((tParams_item*)item->op2)->Param))
                );

				break;

			case ifj16sortCall:

			    #ifdef debug
                    printf("interpreter: call: ifj16sort\n");
                #endif

				*((String*)(item->res)) = sort( *((String*)(((tParams_item*)item->op2)->Param)) );

				break;
			default:
				break;
		}

		switch(instr){

		    case fce_call:

                #ifdef debug
                    printf("interpreter: fce_call: %s called\n",((tIdentifier *)item->op1)->name);
                #endif

				fceVarTree = ( (tLevel *) ( ( (tIdentifier *)(item->op1))->value) )->TreeRoot;
				IdenAllocOrCleanAll(fceVarTree, (tIdentifier *)item->op1, 1);//1 = pruchod stromem a pridani novych instanci promennych

				argCopy((tIdentifier *)(item->op1), (tParams_item *)(item->op2));

				setRetPosAlloc((tIdentifier *)(item->op1), item);

				item = JumpInFce( (tIdentifier *)(item->op1) );

				break;

			case return_val:

                #ifdef debug
                    printf("interpreter: return: retuning from %s\n",((tIdentifier *)item->op1)->name);
                #endif

				fceVarTree = ( (tLevel *) ( ( (tIdentifier *)(item->op1))->value) )->TreeRoot;

				IdenAllocOrCleanAll(fceVarTree, (tIdentifier *)item->op1, 0);//0 = pruchod stromem a uvolneni instanci promennych

				tempitem = item;
				item = JumpOutFce( (tIdentifier *)(item->op1) );
                ReturnClean((tIdentifier *)tempitem->op1, tempitem->op2);

				break;

            case fce_end:

                #ifdef debug
                    printf("interpreter: return at end of function: retuning from %s\n",((tIdentifier *)item->op1)->name);
                #endif

                if (((tIdentifier *)item->op1)->type != _void_) print_error(err_run_uni);
                if (((tIdentifier *)item->op1) == MainFceId && ((tIdentifier *)item->op1)->retValues == NULL) return;

                fceVarTree = ( (tLevel *) ( ( (tIdentifier *)(item->op1))->value) )->TreeRoot;
                IdenAllocOrCleanAll(fceVarTree, (tIdentifier *)tempitem->op1, 0);//0 = pruchod stromem a uvolneni instanci promennych

                tempitem = item;
                item = JumpOutFce( (tIdentifier *)(item->op1) );
                ReturnClean((tIdentifier *)tempitem->op1, tempitem->op2);

				break;

			case if_start:

				if( *((unsigned char*)(item->op1)) ){
					#ifdef debug
						printf("interpreter: control: if %d -> true\n",*(unsigned char*)item->op1);
					#endif
					item = item->next;
				}
				else{
					#ifdef debug
						printf("interpreter: control: if %d -> false\n",*(unsigned char*)item->op1);
					#endif
					item = item->nope;
				}

				break;

			case while_condition:

				if( *((unsigned char*)(item->op1)) ){
					#ifdef debug
						printf("interpreter: control: while %d -> true\n",*(unsigned char*)item->op1);
					#endif
					item = item->next;
				}
				else{
					#ifdef debug
						printf("interpreter: control: while %d -> false\n",*(unsigned char*)item->op1);
					#endif
					item = (item->nope)->next;
				}

				break;

			case while_end:

				item = item->nope;
				#ifdef debug
						printf("interpreter: control: while end\n");
				#endif

				break;

			default:
				item = item->next;
				break;
		}
	}
}

void globalVarHandle(tBST *BST){

	BSTSetActiveTop(BST);

	tMember *MainClassMember = BSTSearchMember(BST, "Main");
	BST->Depth++;
	BST->Active = MainClassMember->NextLevel;

	tMember *runFceMember = BSTSearchMember(BST, "run");
	tLevel *runFceLevel = BSTCreateFindLevel(BST, runFceMember);

	TInstr *item = runFceLevel->fceStart;
    String tempstr = NULL;
	int inFce = 0;

	int i;//pomocna promenna

	while(item != NULL){

		InstrTypes instr = item->t;

		switch(instr){

			case fce_start:

				inFce++;

				break;

			case fce_end:

				if(inFce){

					inFce--;
				}
				else{
					print_error(err_sem_und);
				}

				break;

			default:
				break;
		}

		if(!inFce){
			switch(instr){

				case ass_int:
					*((int*)(item->res)) = *((int*)(item->op1));

					#ifdef debug
						printf("interpreter: assigning int %d\n",*((int*)(item->op1)));
					#endif

					break;

				case eq_int:

					if(*((int*)(item->op1)) == *((int*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %d == %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case noteq_int:

					if(*((int*)(item->op1)) != *((int*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %d == %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case gt_int:

					if(*((int*)(item->op1)) > *((int*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %d > %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case ls_int:

					if(*((int*)(item->op1)) < *((int*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %d < %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case lseq_int:

					if(*((int*)(item->op1)) <= *((int*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %d <= %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case gteq_int:

					if(*((int*)(item->op1)) >= *((int*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					break;

					#ifdef debug
						printf("interpreter: op: %d >= %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

				case add_int:


					*((int*)(item->res)) = (*((int*)(item->op1))) + (*((int*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %d + %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case sub_int:


					*((int*)(item->res)) = (*((int*)(item->op1))) - (*((int*)(item->op2)));

						#ifdef debug
						printf("interpreter: op: %d - %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
						#endif

					break;

				case div_int:

					*((int*)(item->res)) = (*((int*)(item->op1))) / (*((int*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %d / %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;

				case mul_int:

					*((int*)(item->res)) = (*((int*)(item->op1))) * (*((int*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %d * %d\n", *((int*)(item->op1)), *((int*)(item->op2)));
					#endif

					break;
			   case  ass_dbl:

					*((double*)(item->res)) = *((double*)(item->op1));
					break;

					#ifdef debug
						printf("interpreter: op: assigning double\n");
					#endif

				case eq_dbl:

					if(*((double*)(item->op1)) == *((double*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %f == %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case noteq_dbl:

					if(*((double*)(item->op1)) != *((double*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %f != %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case gt_dbl:

					if(*((double*)(item->op1)) > *((double*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %f > %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case ls_dbl:

					if(*((double*)(item->op1)) < *((double*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %f < %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case lseq_dbl:

					if(*((double*)(item->op1)) <= *((double*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %f <= %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case gteq_dbl:

					if(*((double*)(item->op1)) >= *((double*)(item->op2))){

						*((unsigned char*)(item->res)) = 255;
					}
					else{
						*((unsigned char*)(item->res)) = 0;
					}

					#ifdef debug
						printf("interpreter: op: %f >= %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case add_dbl:

					*((double*)(item->res)) = (*((double*)(item->op1))) + (*((double*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %f + %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case sub_dbl:

					*((double*)(item->res)) = (*((double*)(item->op1))) - (*((double*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %f - %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case div_dbl:

					*((double*)(item->res)) = (*((double*)(item->op1))) / (*((double*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %f / %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case mul_dbl:

					*((double*)(item->res)) = (*((double*)(item->op1))) * (*((double*)(item->op2)));

					#ifdef debug
						printf("interpreter: op: %f * %f\n", *((double*)(item->op1)), *((double*)(item->op2)));
					#endif

					break;

				case ass_str:

					i = 0;
					while( (*( (char**)(item->op1) ))[i] != 0 ){
						i++;
					}
                    *(char**)item->res = realloc(*(char**)item->res,sizeof(char)*(i+2));
                    if (*(char**)item->res == NULL) print_error(err_oth);
					for(; i >= 0; i--){
						( *((char**)(item->res)) )[i] = ( *((char**)(item->op1)) )[i];
					}

					#ifdef debug
						printf("interpreter: op: assigning string %s\n", **((String*)(item->op1)));
					#endif

					break;

				case add_str:

                    #ifdef debug
						printf("interpreter: op: %s + %s\n", **((String*)(item->op1)), **((String*)(item->op2)));
					#endif

					tempstr = concat(*(char **)item->op1, *(char **)item->op2);
                    if (*(String*)item->res != NULL) free(*(String*)item->res);
                    *((String*)(item->res)) = tempstr;

					break;

				default:
					break;
			}
		}
		item = item->next;
	}

	if(inFce){
		print_error(err_sem_und);
	}
}

//vytvori novou instanci lokalni promenne funkce
void fceLocVarCopy(struct identifier *prevVarIden){
	tVarVlCopy *newVarVl = malloc(sizeof(struct varVlCopy));
	if (newVarVl == NULL) print_error(err_oth);


	switch( prevVarIden->type ){

		case _void_:
			newVarVl->value = NULL;
			break;

		case _int_:
			(newVarVl->value) = malloc(sizeof(int));
			if (newVarVl->value == NULL) print_error(err_oth);
			*( (int *)(newVarVl->value) ) = *((int *)(prevVarIden->value));
			break;

		case _double_:
			(newVarVl->value) = malloc(sizeof(double));
			if (newVarVl->value == NULL) print_error(err_oth);
			*( (double *)(newVarVl->value) ) = *((double *)(prevVarIden->value));
			break;

		case _String_:
			(newVarVl->value) = malloc(sizeof(String));
			if (newVarVl->value == NULL) print_error(err_oth);
			*((String *)(newVarVl->value)) = *((String *)(prevVarIden->value));
			break;

		case _boolean_:
			(newVarVl->value) = malloc(sizeof(unsigned char));
			if (newVarVl->value == NULL) print_error(err_oth);
			*((unsigned char *)(newVarVl->value)) = *((unsigned char *)(prevVarIden->value));
			break;

		default:
			break;
	}

	newVarVl->prev = prevVarIden->prevValue;
	prevVarIden->prevValue = newVarVl;
}

void argCopy(struct identifier *fceIdentifier, struct parameters_item *ParaItem){

	//kopirovani hodnot predchoziho identifieru lokalni promenne do noveho
	tLevel *fceLevel = (fceIdentifier->value);

	//identifier lok. promenne (argument) funkce
	tArg *pomArg = (fceLevel->Args);

	tIdentifier *pomArgIden;

	while(pomArg != NULL){

		pomArgIden = pomArg->Arg;

		//hardcopy hodnoty argumentu funkce do jeji lok. promenne
		switch( pomArgIden->type ){

			case _void_:
				pomArgIden->value = NULL;
				break;

			case _int_:
				*( (int *)(pomArgIden->value) ) = *((int *)(ParaItem->Param));
				break;

			case _double_:
				*( (double *)(pomArgIden->value) ) = *((double *)(ParaItem->Param));
				break;

			case _String_:
				*((String *)(pomArgIden->value)) = *((String *)(ParaItem->Param));
				break;

			case _boolean_:
				*((unsigned char *)(pomArgIden->value)) = *((bool *)(ParaItem->Param));
				break;

			default:
				break;
		}

		pomArg = pomArg->Next;
	}
}

//vytvori struct retValues pro navatovou hodnotu a misto kam se v kodu vracet
void setRetPosAlloc(struct identifier *fceIdentifier, struct SInstr *item){

	tRetValues *newRetValues = malloc(sizeof(struct retValues));

	if(newRetValues == NULL){
		print_error(err_oth);
	}

	newRetValues->retVal = item->res;
	newRetValues->type = fceIdentifier->type;
	newRetValues->retPos = item->next;

	newRetValues->next = fceIdentifier->retValues;
	(fceIdentifier->retValues) = newRetValues;
}

struct SInstr* JumpInFce(struct identifier *fceIdentifier){

	return (( (tLevel *)(fceIdentifier->value) )->fceStart);
}

struct SInstr* JumpOutFce(struct identifier *fceIdentifier){

	return (TInstr *)((fceIdentifier->retValues)->retPos);
}

//uvolni identifier lokalni promenne funkce
void fceLocVarClean(struct identifier *VarIden){

	tVarVlCopy *prevVarVl = VarIden->prevValue;

	if(prevVarVl != NULL){

		switch( VarIden->type ){

			case _void_:
				VarIden->value = NULL;
				break;

			case _int_:
				*( (int *)(VarIden->value) ) = *((int *)(prevVarVl->value));
				break;

			case _double_:
				*( (double *)(VarIden->value) ) = *((double *)(prevVarVl->value));
				break;

			case _String_:
				*((String *)(VarIden->value)) = *((String *)(prevVarVl->value));
				break;

			case _boolean_:
				*((unsigned char *)(VarIden->value)) = *((unsigned char *)(prevVarVl->value));
				break;

			default:
				break;
		}
	}

	VarIden->prevValue = prevVarVl->prev;

	if(prevVarVl->value != NULL){

		free(prevVarVl->value);
	}

	free(prevVarVl);
}

//ulozi vracenou hodnotu funkce a uvolni struct retValues
void ReturnClean(struct identifier *fceIdentifier, void *retValue){

    if(retValue != NULL){
        switch((fceIdentifier->retValues)->type){

            case _int_:

                *( (int *)((fceIdentifier->retValues)->retVal) ) = *((int *)retValue);
                break;

            case _double_:

                *( (double *)((fceIdentifier->retValues)->retVal) ) = *((double *)retValue);
                break;

            case _boolean_:

                *( (unsigned char *)((fceIdentifier->retValues)->retVal) ) = *((bool *)retValue);
                break;

            case  _String_:

                *( (String *)(fceIdentifier->retValues)->retVal ) = *((String *)retValue);
                break;

            default:
                break;
        }
    }
	tRetValues *actRetValuesStruct = fceIdentifier->retValues;

	fceIdentifier->retValues = actRetValuesStruct->next;

	free(actRetValuesStruct);
}

void IdenAllocOrCleanAll(struct identifier *IdenRootPtr, tIdentifier* fncident, char allocOrClean){

    if (IdenRootPtr == NULL) return;
	if(allocOrClean){
            if (fncident->retValues != NULL) fceLocVarCopy(IdenRootPtr);
		}
		else{
		    if (fncident->retValues->next!= NULL) fceLocVarClean(IdenRootPtr);
		}

	IdenAllocOrCleanAll(IdenRootPtr->LPtr, fncident, allocOrClean);
	IdenAllocOrCleanAll(IdenRootPtr->RPtr, fncident, allocOrClean);
}
