void Save()
{
	FILE *save;

	save = fopen("sokoban.txt", "w"); //쓰기 형식
	
	fprintf(save, "%s\n", USER);
	fprintf(save, "%d\n", undocount);
	fprintf(save, "%d\n", move);
	fprintf(save, "%d\n", stage);
	
	for(int i = 0; i < 30; i++)
	{
		for(int j = 0; j < 30; j++)
		{
			fprintf(save, "%c", map[stage][i][j]);
		}
	fprintf(save, "\n");
	}

	fclose(save);
}
