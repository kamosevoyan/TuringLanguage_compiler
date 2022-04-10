main(_,0,1,*)
{
	ifnot(0,1)
		error;
		
	while(_,0,1,*)
	{
		if (_)
		{
			right;
			continue;
		}
		else if (0)
		{
			write(1);
			right;
		}
		else if(1)
		{
			write(0);
			right;
		}
		else if(*)
		{
			break;
		}
	}
	
	exit;
}
