# // ****************************************************************************
# // copyright (c) 2000-2006 Horst Knorr <hk_classes@knoda.org>
# // This file is part of the hk_classes library.
# // This file may be distributed and/or modified under the terms of the
# // GNU Library Public License version 2 as published by the Free Software
# // Foundation and appearing in the file COPYING included in the
# // packaging of this file.
# // This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# // WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
# // ****************************************************************************
try:	
	import sys,types,time
	from traceback import *
	from os.path import *
	sys.path.append("../python")
except:
	print "Main python modules could not be loaded, wrong installation?"
	sys.exit(1)
try:
	from hk_classes import *
except:
	print "hk_classes module could not be loaded, wrong installation?"
	sys.exit(1)


dr=hk_drivermanager()
if dr==None:
	print "drivermanager could not be loaded!"
	sys.exit(1)

#general definitions
successfull_tests=0
failed_tests=0
testnumber=0
#test_messages=[]
skipped_tests=0
skipped_messages=[]
tested_drivers=[]
performed_tests=[]
test_warnings=[]
reason=""
ok="OK"
failed="FAILED"
skipped="SKIPPED"

defaultdbname="hkclassestest"
testtablename="test"
dependingtablename="depending"



skip_tests={}
additional_tests={}

# Definition of the test table
# columntype,columnname,size,is_primary,is_notnull
column_definitions= [ 
[hk_column.auto_inccolumn,"id",4,True,False],
[hk_column.textcolumn,"text",255,False,False],
[hk_column.memocolumn,"memo",0,False,False],
[hk_column.floatingcolumn,"floating",0,False,False],
[hk_column.smallintegercolumn,"smallint",0,False,False],
[hk_column.datecolumn,"datecol",0,False,False],
[hk_column.timecolumn,"timecol",0,False,False],
[hk_column.datetimecolumn,"dtcol",0,False,False],
[hk_column.boolcolumn,"boolcol",0,False,False]
]



#test data for creating and updating tables
table_columns=["text","memo","smallint","floating","datecol","timecol","dtcol","boolcol"]
table_values= [
["mytext","mymemo",0,0.0,"05.03.2005","14:35:00","01.01.1999 00:00:00",True],
["mytext2","mymemo2",255,255.75,"07.02.1901","00:01:00","31.01.2005 13:42:04",False],
["mytext3","mymemo3",-255,-255.75,"28.11.2006","03:00:15","24.12.2005 15:03:00",True]
]

update_values= [
["myreplacedtext","myreplacedmemo1",5,9.1,"25.01.1974","12:00:00","11.08.1912 03:04:05",False],
["myreplacedt2","myreplacedmemo2",-2,-8.75,"01.03.1965","01:00:14","16.01.1913 13:24:35",True]
]


dependingcolumn_definitions= [ 
[hk_column.auto_inccolumn,"id",4,True,False],
[hk_column.smallintegercolumn,"dep",0,False,False]
]

dependingtable_columns=["dep"]
dependingtable_values= [
[1],[1],[1],[1],
[2],[2],[2],
[3],[3]
]






#special treatment of login infos
# drivername:{database,logindatabase,user,password,host}
login_info={
"firebird":{"database":"/opt/firebird/examples/hkclassestest.fdb","logindatabase":"/unknown.fdb"},
"mdb":{"database":"./misc/test.mdb"}
}


#general test unit function definitions

def perform_test(driver,tstfunction,functiondata,description):
	global failed_tests
	global successfull_tests
	global skipped_tests
	global skipped_messages
	global test_messages
	global performed_tests
	global reason
	global tested_drivers
	global testnumber
	testnumber=testnumber+1
	if driver not in tested_drivers:
		tested_drivers.append(driver)
	
	if driver in skip_tests:
		tsts=skip_tests[driver]
		if tstfunction in tsts:
			reason="Test skipped intentionally"
			msg="["+str(testnumber)+"]\tDriver:'"+driver+"': "+description+"..."+skipped
			skipped_messages.append(msg)
			skipped_messages.append("\t"+reason)
			skipped_tests=skipped_tests+1
			return skipped

	
	
	result=failed
	reason="Unknown"
	try:
		result=tstfunction(driver,functiondata)
	except Exception,inst:
		result=failed
		reason="Exception occured: "+str(inst)
		#print_tb(sys.exc_info(),5)
		
	msg="["+str(testnumber)+"]\tDriver:'"+driver+"': "+description+"..."+result
	print msg
	performed_tests.append(msg)

	if result==failed:
		#print "   Reason: ",reason
		failed_tests=failed_tests+1
		#performed_tests.append("\t"+reason)
		test_warnings.append("["+str(testnumber)+"]\tDriver:'"+driver+"': "+description+": "+reason)
	elif result==ok:
		successfull_tests=successfull_tests+1
	elif result==skipped:
		skipped_tests=skipped_tests+1
		#print "   Reason: ", reason
		#performed_tests.append("\t"+reason)
		skipped_messages.append(msg)
		skipped_messages.append("\t"+reason)
	return result

def print_teststatistic():
	global failed_tests
	global successfull_tests
	global test_messages
	global skipped_tests
	global performed_tests
	line="================================================"
	print line
	print "HK_CLASSES Test Suite"
	print "Test summary at",time.strftime("%d.%m.%y at %H:%M")
	print line
	print "Tested drivers:",tested_drivers
	print "Total tests:\t",successfull_tests+failed_tests+skipped_tests
	print "Failed: \t",failed_tests
	print "Skipped:\t",skipped_tests
	print line
	print "Performed tests:"
	print line
	for tst in performed_tests:
		print tst
	if len(test_warnings)>0:
		print
		print "Failed tests:"
		print line
		for tst in test_warnings:
			print tst
		print 
	if len(skipped_messages)>0:
		print
		print "Skipped tests:"
		print line
		for tst in skipped_messages:
			print tst
		print 

	

#FUNCTIONALITY FUNCTIONS
def testdbname(driver):
	result=defaultdbname
	if driver in login_info:
		login=login_info[driver]
		if "database" in login:
			result=login["database"]
	return result

def new_connection(driver):
	global dr
	global reason
	global login_info
	l=dr.driverlist()
	if not  driver in l:
		reason="No such driver:'"+driver+"'"
		return skipped
	con=dr.new_connection(driver)
	if con == None:
		reason= "Connection was not possible"
		return skipped
	
	if driver in login_info:
		login=login_info[driver]
		if "logindatabase" in login:
			con.set_defaultdatabase(login["logindatabase"])
		if "user" in login:
			con.set_user(login["user"])
		if "password" in login:
			con.set_password(login["password"])
		if "host" in login:
			con.set_password(login["host"])
			
	
	con.connect(hk_class.noninteractive)
	if not con.is_connected():
		reason="Connection can't connect:"+con.last_servermessage()
		return skipped
	return con

def create_database(con,db,deleteexisting=False):
	global reason
	if not con.server_supports(hk_connection.SUPPORTS_NEW_DATABASE):
		reason="Server does not support creating new databases"
		return skipped
	if (db in con.dblist()) and deleteexisting:
		con.delete_database(db,hk_class.noninteractive)
	result=con.create_database(db)
	if result ==1:
		return ok
	else:
		reason=con.last_servermessage()
		return failed





def open_database(driver,dbname,create=False,deleteexistingdb=False,user="root",password="",host=""):
	global dr
	global reason
	con=new_connection(driver)
	if con==skipped:
		return skipped
	if create and (not dbname in con.dblist() and not exists(dbname)):
		result=create_database(con,dbname,deleteexistingdb)
		if result!=ok:
			reason="create_database failed: "+con.last_servermessage()
			return result
	
	if (not dbname in con.dblist() and not exists(dbname)):
		reason="database '"+dbname+"' does not exist"
		
		return failed
	db=con.new_database(dbname)
	if db==None:
		reason= "Database could not be created: "+con.last_servermessage()
		return failed
	return db




def delete_database(con,db):
	global reason
	if not con.server_supports(hk_connection.SUPPORTS_DELETE_DATABASE):
		reason="Server does not support deleting databases"
		return skipped
	if (not db in con.dblist()):
		reason="Database '"+db+"' does not exist, dblist="+str(con.dblist())
		return failed
	result=con.delete_database(db,hk_class.noninteractive)
	if result ==1:
		return ok
	else:
		reason=con.last_servermessage()
		return failed



def create_table(db,tbl,coldef,values,valuecolumns,insert=False):
	global reason
	if not db.connection().server_supports(hk_connection.SUPPORTS_NEW_TABLE):
		reason="Server does not support creating new tables"
		return skipped
	if tbl in db.tablelist():
		db.delete_table(tbl,False)
	table=db.new_table()
	table.setmode_createtable()
	table.set_name(tbl)
	for col in coldef:
		c=table.new_column()
		c.set_columntype(col[0])
		c.set_name(col[1])
		c.set_size(col[2])
		c.set_primary(col[3])
		c.set_notnull(col[4])
	result=table.create_table_now()
	if result ==1:
		if insert:
			insert_table(table,values,valuecolumns)
		return table
	else:
		return None




def delete_table(db,tbl):
	global reason
	if not db.connection().server_supports(hk_connection.SUPPORTS_DELETE_TABLE):
		reason="Server does not support deleting tables"
		return skipped
	if not tbl in db.tablelist():
		reason="Table does not exist"
		return failed
	if db.delete_table(tbl,False):
		return ok
	else:
		reason="Deleting table failed"
		return failed



def insert_table(tbl,values,columns):
	global reason
	if not tbl.is_enabled():
		tbl.enable()
		if not tbl.is_enabled():
			reason="Table could not be enabled"
			return failed
	rownumber=0
	for row in values:
		colid=0
		tbl.setmode_insertrow()
		for col in row:
			c=tbl.column_by_name(columns[colid])
			t=type(col)
			if t== types.IntType:
				c.set_asinteger(col)
			elif t== types.FloatType:
				c.set_asdouble(col)
			elif t== types.BooleanType:
				c.set_asbool(col)
			else:
				c.set_asstring(col)
			colid=colid+1
		if not tbl.store_changed_data():
			reason="Could not add row:"+str(rownumber)+" "+tbl.database().connection().last_servermessage()
			tbl.set_ignore_changed_data()
			tbl.disable()
			return failed
		rownumber=rownumber+1
	tbl.disable()
	return ok


def update_table(tbl):
	global reason
	if not tbl.is_enabled():
		tbl.enable()
		if not tbl.is_enabled():
			reason="Table could not be enabled"
			return failed
	rownumber=0
	if tbl.max_rows()<len(update_values):
		reason="not enough rows"
		return failed
	tbl.goto_first()
	for row in update_values:
		colid=0
		#tbl.setmode_insertrow()
		for col in row:
			c=tbl.column_by_name(table_columns[colid])
			t=type(col)
			if t== types.IntType:
				c.set_asinteger(col)
			elif t== types.FloatType:
				c.set_asdouble(col)
			elif t== types.BooleanType:
				c.set_asbool(col)
			else:
				c.set_asstring(col)
			colid=colid+1
		if not tbl.store_changed_data():
			reason="Could not update row:"+str(rownumber)+" "+tbl.database().connection().last_servermessage()
			tbl.set_ignore_changed_data()
			tbl.disable()
			return failed
		rownumber=rownumber+1
		tbl.goto_next()
	tbl.disable()
	return ok


def delete_multiplerows(tbl):
	global reason
	startrow=0
	deleterows=2
	if not tbl.is_enabled():
		tbl.enable()
		if not tbl.is_enabled():
			reason="Table could not be enabled"
			return failed
	max=tbl.max_rows()
	if tbl.max_rows()<deleterows:
		reason="not enough rows"
		return failed
	result=tbl.delete_rows(startrow,startrow+deleterows-1,True,hk_class.noninteractive)
	tbl.disable()
	tbl.enable()
	newmax=tbl.max_rows()
	tbl.disable()
	max=max-deleterows
	if max!=newmax:
		reason="Table has not expected number of rows. It has "+str(newmax)+", it should have "+str(max)+"\n"+tablecontent(table)
		return failed
	if result==True:	
		return ok
	else:
		reason="delete_rows() returns False"
		return failed



def delete_currentrow(tbl):
	global reason
	if not tbl.is_enabled():
		tbl.enable()
		if not tbl.is_enabled():
			reason="Table could not be enabled"
			return failed
	if tbl.max_rows()<1:
		reason="not enough rows"
		return failed
	result=tbl.delete_rows(0,0,True,hk_class.noninteractive)
	tbl.disable()
	if result==True:	
		return ok
	else:
		reason="delete_row() returns False"
		return failed


def create_dependingdatasource(driver,tblname):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	
	res=create_table(db,tblname,dependingcolumn_definitions,dependingtable_values,dependingtable_columns,True)

	con=db.connection()
	del db
	con.disconnect()
	del con
	if res!=None:
		return ok
	return failed




def print_table(tbl):
	i=0
	print "PRINT TABLE:",tbl.name(),"Driver:",tbl.database().connection().drivername()
	print "========================"
	if not tbl.is_enabled():
		tbl.enable()
	while i <tbl.max_rows():
		print tbl.show_currentrow()
		tbl.goto_next()
		i=i+1
	print "========================"

def tablecontent(tbl):
	i=0
	result=""
	result=result+ "\n\tTABLE: "+tbl.name()+" Driver:"+tbl.database().connection().drivername()
	result=result+"\n\t========================"
	if not tbl.is_enabled():
		tbl.enable()
	while i <tbl.max_rows():
		result=result+"\n\t"+str( tbl.show_currentrow())
		tbl.goto_next()
		i=i+1
	result=result+ "\n\t========================"
	return result+"\n"



# TESTFUNCTION DEFINITIONS 
def tst_connection(driver,data):
	global dr
	con=new_connection(driver)
	if con==skipped:
		return failed
	else:
		con.disconnect()
		del con
		return ok
	
def tst_createdatabase(driver,data):
	global dr
	con=new_connection(driver)
	if con==skipped:
		return skipped
	result= create_database(con,defaultdbname,True)
	del con
	return result
	#db=open_database(driver,testdbname(driver),True,True)

def tst_opendatabase(driver,data):
	global dr
	global reason
	#con=dr.new_connection(driver)
	db=open_database(driver,data)
	if db==failed or db==skipped:
		return skipped
	else:
		con=db.connection()
		del db
		con.disconnect()
		del con
		return ok



def tst_deletedatabase(driver,data):
	global dr
	con=new_connection(driver)
	if con==skipped:
		return skipped
	result=delete_database(con,data)
	con.disconnect()
	del con
	return result





def tst_createtable(driver,data):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	else:
		result= create_table(db,data,column_definitions,table_values,table_columns,False)
		con=db.connection()
		del db
		con.disconnect()
		del con
		if result==failed or result==skipped:
			return result
		else:
			return ok

def tst_deletetable(driver,data):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	else:
		result= delete_table(db,data)
		con=db.connection()
		del db
		con.disconnect()
		del con
		if result==failed or result==skipped:
			return result
		else:
			return ok




def tst_insertdata(driver,data):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	table=db.new_table()
	if not table:
		reason="Table object could not be created"
		return skipped
	table.set_name(data)
	res=insert_table(table,table_values,table_columns)
	con=db.connection()
	del db
	con.disconnect()
	del con
	return res




def tst_updatedata(driver,data):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	table=db.new_table()
	if not table:
		reason="Table object could not be created"
		return skipped
	table.set_name(data)
	res=update_table(table)
	con=db.connection()
	del db
	con.disconnect()
	del con
	return res




def tst_deletemultiplerows(driver,data):
	global dr
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	table=db.new_table()
	if not table:
		reason="Table object could not be created"
		return skipped
	table.set_name(data)
	res=delete_multiplerows(table)
	con=db.connection()
	del db
	con.disconnect()
	del con
	return res

def tst_deletesinglerow(driver,data):
	global dr
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	if (db==None):
		return failed
	table=db.new_table()
	if not table:
		reason="Table object could not be created"
		del db
		return skipped
	table.set_name(data)
	res=delete_currentrow(table)
	con=db.connection()
	del db
	con.disconnect()
	del con
	return res

def tst_comparedata(driver,tablename):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	table=db.new_table()
	if not table:
		reason="Table object could not be created"
		del db
		return skipped
	table.set_name(tablename)
	i=0
	if not table.is_enabled():
		table.enable()

	if not table.is_enabled():
		reason="Datasource '"+tablename+"' could not be enabled"
		del db
		return failed
	table.goto_first()
 	if len(table_values)!=table.max_rows():
 		reason="different amount of rows. Table has "+str(table.max_rows())+", but should have "+str(len(table_values))+tablecontent(table)
		del db
 		return failed
	
	rownumber=0
	for row in table_values:
		colid=0
		for col in row:
			c=table.column_by_name(table_columns[colid])
			if c==None:
				reason="Column:'"+table_columns[colid]+"' not found"
				del db
				return failed
			t=type(col)
			if t== types.IntType:
				if c.asinteger()!=col:
					reason="Row:"+str(rownumber)+" Integer in column:'"+table_columns[colid]+"' is different\n\t"
					reason=reason+"Table has '"+str(c.asinteger())+"', should be '"+str(col)+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			elif t== types.FloatType:
				if c.asdouble()!=col:
					reason="Row:"+str(rownumber)+" Float in column:'"+table_columns[colid]+"' is different\n\t"
					print reason
					reason=reason+"Table has '"+str(c.asdouble())+"', should be '"+str(col)+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			elif t== types.BooleanType:
				if c.asinteger()!=col:
					reason="Row:"+str(rownumber)+" Bool in column:'"+table_columns[colid]+"' is different\n\t"
					print reason
					reason=reason+"Table has '"+str(c.asbool())+"', should be '"+str(col)+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			else:
				if c.asstring()!=col:
					reason="Row:"+str(rownumber)+" String in column:'"+table_columns[colid]+"' is different\n\t"
					reason=reason+"Table has '"+c.asstring()+"', should be '"+col+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			colid=colid+1
		table.goto_next()
		rownumber=rownumber+1
	table.disable()
	con=db.connection()
	del db
	con.disconnect()
	del con
	return ok




def tst_compareupdatedata(driver,tablename):
	global dr
	global reason
	db=open_database(driver,testdbname(driver))
	if db==failed or db==skipped:
		return skipped
	table=db.new_table()
	if not table:
		reason="Table object could not be created"
		del db
		return skipped
	table.set_name(tablename)
	i=0
	table.disable()
	table.set_sorting("\"id\"")
	if not table.is_enabled():
		table.enable()

	if not table.is_enabled():
		reason="Datasource '"+tablename+"' could not be enabled"
		del db
		return failed
 	if len(update_values)>table.max_rows():
 		reason="different amount of rows. Table has "+str(table.max_rows())+", but should have "+str(len(update_values))+tablecontent(table)
		del db
 		return failed
	
	rownumber=0
	for row in update_values:
		colid=0
		for col in row:
			c=table.column_by_name(table_columns[colid])
			if c==None:
				reason="Column:'"+table_columns[colid]+"' not found"
				return failed
			t=type(col)
			if t== types.IntType:
				if c.asinteger()!=col:
					reason="Row:"+str(rownumber)+" Integer in column:'"+table_columns[colid]+"' is different\n\t"
					reason=reason+"Table has '"+str(c.asinteger())+"', should be '"+str(col)+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			elif t== types.FloatType:
				if c.asdouble()!=col:
					reason="Row:"+str(rownumber)+" Float in column:'"+table_columns[colid]+"' is different\n\t"
					reason=reason+"Table has '"+str(c.asdouble())+"', should be '"+str(col)+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			elif t== types.BooleanType:
				if c.asbool()!=col:
					reason="Row:"+str(rownumber)+" Bool in column:'"+table_columns[colid]+"' is different\n\t"
					reason=reason+"Table has '"+str(c.asbool())+"', should be '"+str(col)+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			else:
				if c.asstring()!=col:
					reason="Row:"+str(rownumber)+" String in column:'"+table_columns[colid]+"' is different\n\t"
					reason=reason+"Table has '"+c.asstring()+"', should be '"+col+"'"+tablecontent(table)
					table.disable()
					del db
					return failed
			colid=colid+1
		table.goto_next()
		rownumber=rownumber+1
	table.disable()
	con=db.connection()
	del db
	con.disconnect()
	del con
	return ok

filename='hktst.csv'
coldel="\t"
def create_cvsfile():
	file=open(filename,'w')
	if file==None:
		reason="csv file could not be created for writing"
		return failed
	
	c=0
	for colname in table_columns:
		if c>0:
			file.write(coldel)
		file.write('"'+colname+'"')
		c=c+1
	file.write("\n")
	
	for row in table_values:
		colid=0
		for col in row:
			if colid>0:
				file.write(coldel)
			t=type(col)
			if t== types.IntType:
				file.write(str(col))
			elif t== types.FloatType:
				file.write(str(col))
			elif t== types.BooleanType:
				file.write(str(col))
			else:
				file.write('"'+str(col)+'"')
			colid=colid+1
		file.write("\n")
	file.close()
	

def tst_importcsv(driver,data):
	global dr
	global reason
	create=True
 	db=open_database(driver,testdbname(driver),create)
 	if db==failed or db==skipped:
		return skipped
	i=hk_importcsv()
	t=db.new_table()
# 	t.set_print_sqlstatements(True)
	t.set_name(testtablename)
	i.set_datasource(t)
	i.set_filename(filename)
	i.set_firstrow_contains_fieldnames(True)
	i.set_overwrite_table(True)
	

	
	i.set_filedefinition('"',coldel,"\n","D.M.Y h:m:s","D.M.Y","h:m:s","","C")
	
	if not i.execute(hk_class.noninteractive):
		reason="execute failed"
		return failed
	t.disable()
	del t
	del i
	res=tst_comparedata(driver,testtablename)
	if res !=ok:
		return res
# 	res=tst_deletedatabase(driver,testdbname(driver))
# 	if res!=ok:
# 		return res
	return ok





def tst_dependingdatasource(driver,data):
	global dr
	global reason
 	db=open_database(driver,testdbname(driver))
 	if db==failed or db==skipped:
		return skipped
	res=create_dependingdatasource(driver,dependingtablename)
	if res==failed:
		return failed
	print db.tablelist()
	master=db.new_table(testtablename)
	slave=db.new_table(dependingtablename)
	slave.set_depending_on(master)
	slave.add_depending_fields("dep","id")
	if not master.enable():
		reason="Master table could not be enabled"
		return failed
	if not slave.is_enabled():
		reason="Slave table is not enabled"
		return failed
	col=master.column_by_name("id")
	if col==None:
		reason="Master id column not found"
		return failed
	for i in xrange(2):
		v=dependingtable_values.count([col.asinteger()])
		if slave.max_rows()!=v:
			reason="Different amount of rows, Slave has "+str(slave.max_rows())+", but should have "+str(v)
			return failed
		master.goto_next()
	master.disable()
	slave.disable()
	return ok


def perform_fulldrivertest(driver):
	global reason
	data=""
 	result=perform_test(driver,tst_connection,data,"connection test")
	if result==failed:
		return
	result=perform_test(driver,tst_createdatabase,data,"create database test")
 	result=perform_test(driver,tst_opendatabase,testdbname(driver),"open existing database test")
 	result=perform_test(driver,tst_createtable,testtablename,"create table test")
 	result=perform_test(driver,tst_insertdata,testtablename,"insert data test")
	result=perform_test(driver,tst_comparedata,testtablename,"compare inserted data test")
	if result!=ok:
		return
	result=perform_test(driver,tst_updatedata,testtablename,"update data test")
	result=perform_test(driver,tst_compareupdatedata,testtablename,"compare updated data test")
	if result!=ok:
		return
	if driver in additional_tests:
		tsts=additional_tests[driver]
		if tst_dependingdatasource in tsts:
			result=perform_test(driver,tst_dependingdatasource,data,"depending datasource test")
			if result==failed:
				return
		if tst_importcsv in tsts:
			result=perform_test(driver,tst_importcsv,data,"importcsv test")
			if result==failed:
				return
	
	
	result=perform_test(driver,tst_deletemultiplerows,testtablename,"delete multiple rows test")
	result=perform_test(driver,tst_deletesinglerow,testtablename,"delete single row test")
	result=perform_test(driver,tst_deletetable,testtablename,"delete table test")
	result=perform_test(driver,tst_deletedatabase,testdbname(driver),"delete database test")
	
	




#define performed tests
writetests=[tst_insertdata,tst_comparedata,tst_updatedata,tst_compareupdatedata,tst_deletesinglerow,tst_deletemultiplerows,tst_deletetable,tst_deletedatabase]
createtests=[tst_createdatabase,tst_createtable,]
skip_tests["dbase"]=[tst_compareupdatedata]
skip_tests["mdb"]=createtests+writetests
skip_tests["postgres"]=[tst_deletedatabase]

hkclassestests=[tst_importcsv,tst_dependingdatasource]
additional_tests["sqlite3"]=hkclassestests
additional_tests["sqlite2"]=hkclassestests
additional_tests["mysql"]=hkclassestests
additional_tests["postgres"]=hkclassestests
additional_tests["dbase"]=[tst_importcsv]
#additional_tests["paradox"]=[tst_importcsv]


create_cvsfile()
#perform_fulldrivertest("dbase")
for driver in dr.driverlist():
	if driver!="xbase":
		perform_fulldrivertest(driver)
#perform_fulldrivertest("paradox")
#show statistics
print_teststatistic()
