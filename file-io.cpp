	
 void saveFile(const char* name)//sample
 {
 	FILE* fp = NULL;
	fopen_s(&fp,name, "wb");
	if (fp == NULL) 
	{
	   return ;
	}

	int num_node_input = num_node_input_BP;
	int num_node_hidden = num_node_hidden_BP;
	int num_node_output = num_node_output_BP;
	fwrite(&num_node_input, sizeof(int), 1, fp);
	fwrite(&num_node_hidden, sizeof(int), 1, fp);
	fwrite(&num_node_output, sizeof(int), 1, fp);
   	fwrite(weight1, sizeof(float)*num_node_input_BP*num_node_hidden_BP, 1, fp);
 	fwrite(threshold1, sizeof(float)*num_node_hidden_BP, 1, fp);
 	fwrite(weight2, sizeof(float)*num_node_hidden_BP*num_node_output_BP, 1, fp);
	fwrite(threshold2, sizeof(float)*num_node_output_BP, 1, fp);

	fflush(fp);
	fclose(fp);
 }
