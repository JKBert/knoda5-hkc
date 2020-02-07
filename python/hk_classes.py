# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _hk_classes

def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


ft_table = _hk_classes.ft_table
ft_query = _hk_classes.ft_query
ft_form = _hk_classes.ft_form
ft_report = _hk_classes.ft_report
ft_module = _hk_classes.ft_module
ft_view = _hk_classes.ft_view
ft_referentialintegrity = _hk_classes.ft_referentialintegrity
lt_table = _hk_classes.lt_table
lt_query = _hk_classes.lt_query
lt_form = _hk_classes.lt_form
lt_report = _hk_classes.lt_report
lt_index = _hk_classes.lt_index
lt_module = _hk_classes.lt_module
lt_view = _hk_classes.lt_view
dt_table = _hk_classes.dt_table
dt_query = _hk_classes.dt_query
dt_view = _hk_classes.dt_view
ot_database = _hk_classes.ot_database
ot_table = _hk_classes.ot_table
ot_query = _hk_classes.ot_query
ot_form = _hk_classes.ot_form
ot_report = _hk_classes.ot_report
ot_module = _hk_classes.ot_module
ot_view = _hk_classes.ot_view

is_numerictype = _hk_classes.is_numerictype

is_integertype = _hk_classes.is_integertype

is_realtype = _hk_classes.is_realtype

inch2cm = _hk_classes.inch2cm

cm2inch = _hk_classes.cm2inch
class dependingclass(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, dependingclass, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, dependingclass, name)
    def __repr__(self):
        return "<C dependingclass instance at %s>" % (self.this,)
    __swig_setmethods__["dependingfield"] = _hk_classes.dependingclass_dependingfield_set
    __swig_getmethods__["dependingfield"] = _hk_classes.dependingclass_dependingfield_get
    if _newclass:dependingfield = property(_hk_classes.dependingclass_dependingfield_get, _hk_classes.dependingclass_dependingfield_set)
    __swig_setmethods__["masterfield"] = _hk_classes.dependingclass_masterfield_set
    __swig_getmethods__["masterfield"] = _hk_classes.dependingclass_masterfield_get
    if _newclass:masterfield = property(_hk_classes.dependingclass_masterfield_get, _hk_classes.dependingclass_masterfield_set)
    def __init__(self, *args):
        _swig_setattr(self, dependingclass, 'this', _hk_classes.new_dependingclass(*args))
        _swig_setattr(self, dependingclass, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_dependingclass):
        try:
            if self.thisown: destroy(self)
        except: pass

class dependingclassPtr(dependingclass):
    def __init__(self, this):
        _swig_setattr(self, dependingclass, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, dependingclass, 'thisown', 0)
        _swig_setattr(self, dependingclass,self.__class__,dependingclass)
_hk_classes.dependingclass_swigregister(dependingclassPtr)

class struct_raw_data(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, struct_raw_data, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, struct_raw_data, name)
    def __repr__(self):
        return "<C struct_raw_data instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, struct_raw_data, 'this', _hk_classes.new_struct_raw_data(*args))
        _swig_setattr(self, struct_raw_data, 'thisown', 1)
    __swig_setmethods__["length"] = _hk_classes.struct_raw_data_length_set
    __swig_getmethods__["length"] = _hk_classes.struct_raw_data_length_get
    if _newclass:length = property(_hk_classes.struct_raw_data_length_get, _hk_classes.struct_raw_data_length_set)
    __swig_setmethods__["data"] = _hk_classes.struct_raw_data_data_set
    __swig_getmethods__["data"] = _hk_classes.struct_raw_data_data_get
    if _newclass:data = property(_hk_classes.struct_raw_data_data_get, _hk_classes.struct_raw_data_data_set)
    def __del__(self, destroy=_hk_classes.delete_struct_raw_data):
        try:
            if self.thisown: destroy(self)
        except: pass

class struct_raw_dataPtr(struct_raw_data):
    def __init__(self, this):
        _swig_setattr(self, struct_raw_data, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, struct_raw_data, 'thisown', 0)
        _swig_setattr(self, struct_raw_data,self.__class__,struct_raw_data)
_hk_classes.struct_raw_data_swigregister(struct_raw_dataPtr)

class referentialclass(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, referentialclass, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, referentialclass, name)
    def __repr__(self):
        return "<C referentialclass instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, referentialclass, 'this', _hk_classes.new_referentialclass(*args))
        _swig_setattr(self, referentialclass, 'thisown', 1)
    __swig_setmethods__["p_name"] = _hk_classes.referentialclass_p_name_set
    __swig_getmethods__["p_name"] = _hk_classes.referentialclass_p_name_get
    if _newclass:p_name = property(_hk_classes.referentialclass_p_name_get, _hk_classes.referentialclass_p_name_set)
    __swig_setmethods__["p_masterdatasource"] = _hk_classes.referentialclass_p_masterdatasource_set
    __swig_getmethods__["p_masterdatasource"] = _hk_classes.referentialclass_p_masterdatasource_get
    if _newclass:p_masterdatasource = property(_hk_classes.referentialclass_p_masterdatasource_get, _hk_classes.referentialclass_p_masterdatasource_set)
    __swig_setmethods__["p_fields"] = _hk_classes.referentialclass_p_fields_set
    __swig_getmethods__["p_fields"] = _hk_classes.referentialclass_p_fields_get
    if _newclass:p_fields = property(_hk_classes.referentialclass_p_fields_get, _hk_classes.referentialclass_p_fields_set)
    __swig_setmethods__["p_deletecascade"] = _hk_classes.referentialclass_p_deletecascade_set
    __swig_getmethods__["p_deletecascade"] = _hk_classes.referentialclass_p_deletecascade_get
    if _newclass:p_deletecascade = property(_hk_classes.referentialclass_p_deletecascade_get, _hk_classes.referentialclass_p_deletecascade_set)
    __swig_setmethods__["p_updatecascade"] = _hk_classes.referentialclass_p_updatecascade_set
    __swig_getmethods__["p_updatecascade"] = _hk_classes.referentialclass_p_updatecascade_get
    if _newclass:p_updatecascade = property(_hk_classes.referentialclass_p_updatecascade_get, _hk_classes.referentialclass_p_updatecascade_set)
    def __del__(self, destroy=_hk_classes.delete_referentialclass):
        try:
            if self.thisown: destroy(self)
        except: pass

class referentialclassPtr(referentialclass):
    def __init__(self, this):
        _swig_setattr(self, referentialclass, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, referentialclass, 'thisown', 0)
        _swig_setattr(self, referentialclass,self.__class__,referentialclass)
_hk_classes.referentialclass_swigregister(referentialclassPtr)


load_file = _hk_classes.load_file

save_file = _hk_classes.save_file

save_textfile = _hk_classes.save_textfile

file_exists = _hk_classes.file_exists

data2hex = _hk_classes.data2hex

hex2data = _hk_classes.hex2data

smallstringconversion = _hk_classes.smallstringconversion

l2u = _hk_classes.l2u

u2l = _hk_classes.u2l

string2upper = _hk_classes.string2upper

string2lower = _hk_classes.string2lower

localestring2double = _hk_classes.localestring2double

localestring2int = _hk_classes.localestring2int

localestring2uint = _hk_classes.localestring2uint

standardstring2double = _hk_classes.standardstring2double

longint2string = _hk_classes.longint2string

ulongint2string = _hk_classes.ulongint2string

replace_all = _hk_classes.replace_all

trimleft = _hk_classes.trimleft

trimright = _hk_classes.trimright

trim = _hk_classes.trim

bin2hex = _hk_classes.bin2hex

hex2bin = _hk_classes.hex2bin
file_open = _hk_classes.file_open
file_save = _hk_classes.file_save
class hk_class(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_class, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, hk_class, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_class instance at %s>" % (self.this,)
    noninteractive = _hk_classes.hk_class_noninteractive
    interactive = _hk_classes.hk_class_interactive
    __swig_getmethods__["show_warningmessage"] = lambda x: _hk_classes.hk_class_show_warningmessage
    if _newclass:show_warningmessage = staticmethod(_hk_classes.hk_class_show_warningmessage)
    __swig_getmethods__["hk_translate"] = lambda x: _hk_classes.hk_class_hk_translate
    if _newclass:hk_translate = staticmethod(_hk_classes.hk_class_hk_translate)
    __swig_getmethods__["show_yesnodialog"] = lambda x: _hk_classes.hk_class_show_yesnodialog
    if _newclass:show_yesnodialog = staticmethod(_hk_classes.hk_class_show_yesnodialog)
    __swig_getmethods__["show_stringvaluedialog"] = lambda x: _hk_classes.hk_class_show_stringvaluedialog
    if _newclass:show_stringvaluedialog = staticmethod(_hk_classes.hk_class_show_stringvaluedialog)
    __swig_getmethods__["set_filedialog"] = lambda x: _hk_classes.hk_class_set_filedialog
    if _newclass:set_filedialog = staticmethod(_hk_classes.hk_class_set_filedialog)
    __swig_getmethods__["show_filedialog"] = lambda x: _hk_classes.hk_class_show_filedialog
    if _newclass:show_filedialog = staticmethod(_hk_classes.hk_class_show_filedialog)
    __swig_getmethods__["save_preferences"] = lambda x: _hk_classes.hk_class_save_preferences
    if _newclass:save_preferences = staticmethod(_hk_classes.hk_class_save_preferences)
    __swig_getmethods__["set_directorydialog"] = lambda x: _hk_classes.hk_class_set_directorydialog
    if _newclass:set_directorydialog = staticmethod(_hk_classes.hk_class_set_directorydialog)
    __swig_getmethods__["show_directorydialog"] = lambda x: _hk_classes.hk_class_show_directorydialog
    if _newclass:show_directorydialog = staticmethod(_hk_classes.hk_class_show_directorydialog)
    __swig_getmethods__["set_default_automatic_data_update"] = lambda x: _hk_classes.hk_class_set_default_automatic_data_update
    if _newclass:set_default_automatic_data_update = staticmethod(_hk_classes.hk_class_set_default_automatic_data_update)
    __swig_getmethods__["default_automatic_data_update"] = lambda x: _hk_classes.hk_class_default_automatic_data_update
    if _newclass:default_automatic_data_update = staticmethod(_hk_classes.hk_class_default_automatic_data_update)

class hk_classPtr(hk_class):
    def __init__(self, this):
        _swig_setattr(self, hk_class, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_class, 'thisown', 0)
        _swig_setattr(self, hk_class,self.__class__,hk_class)
_hk_classes.hk_class_swigregister(hk_classPtr)

format_number = _hk_classes.format_number

format_standard_number = _hk_classes.format_standard_number

hk_class_show_warningmessage = _hk_classes.hk_class_show_warningmessage

hk_class_hk_translate = _hk_classes.hk_class_hk_translate

hk_class_show_yesnodialog = _hk_classes.hk_class_show_yesnodialog

hk_class_show_stringvaluedialog = _hk_classes.hk_class_show_stringvaluedialog

hk_class_set_filedialog = _hk_classes.hk_class_set_filedialog

hk_class_show_filedialog = _hk_classes.hk_class_show_filedialog

hk_class_save_preferences = _hk_classes.hk_class_save_preferences

hk_class_set_directorydialog = _hk_classes.hk_class_set_directorydialog

hk_class_show_directorydialog = _hk_classes.hk_class_show_directorydialog

hk_class_set_default_automatic_data_update = _hk_classes.hk_class_set_default_automatic_data_update

hk_class_default_automatic_data_update = _hk_classes.hk_class_default_automatic_data_update


show_warningmessage = _hk_classes.show_warningmessage

show_yesnodialog = _hk_classes.show_yesnodialog

show_filedialog = _hk_classes.show_filedialog

show_stringvaluedialog = _hk_classes.show_stringvaluedialog

show_directorydialog = _hk_classes.show_directorydialog
class hk_connection(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_connection, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_connection, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_connection instance at %s>" % (self.this,)
    def __del__(self, destroy=_hk_classes.delete_hk_connection):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_host(*args): return _hk_classes.hk_connection_set_host(*args)
    def host(*args): return _hk_classes.hk_connection_host(*args)
    def set_user(*args): return _hk_classes.hk_connection_set_user(*args)
    def user(*args): return _hk_classes.hk_connection_user(*args)
    def set_password(*args): return _hk_classes.hk_connection_set_password(*args)
    def set_tcp_port(*args): return _hk_classes.hk_connection_set_tcp_port(*args)
    def tcp_port(*args): return _hk_classes.hk_connection_tcp_port(*args)
    def default_tcp_port(*args): return _hk_classes.hk_connection_default_tcp_port(*args)
    def set_defaultdatabase(*args): return _hk_classes.hk_connection_set_defaultdatabase(*args)
    def defaultdatabase(*args): return _hk_classes.hk_connection_defaultdatabase(*args)
    def connect(*args): return _hk_classes.hk_connection_connect(*args)
    def disconnect(*args): return _hk_classes.hk_connection_disconnect(*args)
    def is_connected(*args): return _hk_classes.hk_connection_is_connected(*args)
    def dblist(*args): return _hk_classes.hk_connection_dblist(*args)
    def new_database(*args): return _hk_classes.hk_connection_new_database(*args)
    def create_database(*args): return _hk_classes.hk_connection_create_database(*args)
    def delete_database(*args): return _hk_classes.hk_connection_delete_database(*args)
    def database_exists(*args): return _hk_classes.hk_connection_database_exists(*args)
    def drivermanager(*args): return _hk_classes.hk_connection_drivermanager(*args)
    __swig_getmethods__["check_capabilities"] = lambda x: _hk_classes.hk_connection_check_capabilities
    if _newclass:check_capabilities = staticmethod(_hk_classes.hk_connection_check_capabilities)
    def rename_database(*args): return _hk_classes.hk_connection_rename_database(*args)
    SUPPORTS_AUTOINCCOLUMN = _hk_classes.hk_connection_SUPPORTS_AUTOINCCOLUMN
    SUPPORTS_BOOLCOLUMN = _hk_classes.hk_connection_SUPPORTS_BOOLCOLUMN
    SUPPORTS_DATECOLUMN = _hk_classes.hk_connection_SUPPORTS_DATECOLUMN
    SUPPORTS_TIMECOLUMN = _hk_classes.hk_connection_SUPPORTS_TIMECOLUMN
    SUPPORTS_DATETIMECOLUMN = _hk_classes.hk_connection_SUPPORTS_DATETIMECOLUMN
    SUPPORTS_BINARYCOLUMN = _hk_classes.hk_connection_SUPPORTS_BINARYCOLUMN
    SUPPORTS_MEMOCOLUMN = _hk_classes.hk_connection_SUPPORTS_MEMOCOLUMN
    SUPPORTS_TIMESTAMPCOLUMN = _hk_classes.hk_connection_SUPPORTS_TIMESTAMPCOLUMN
    SUPPORTS_TEXTCOLUMN = _hk_classes.hk_connection_SUPPORTS_TEXTCOLUMN
    SUPPORTS_INTEGERCOLUMN = _hk_classes.hk_connection_SUPPORTS_INTEGERCOLUMN
    SUPPORTS_SMALLINTEGERCOLUMN = _hk_classes.hk_connection_SUPPORTS_SMALLINTEGERCOLUMN
    SUPPORTS_FLOATINGCOLUMN = _hk_classes.hk_connection_SUPPORTS_FLOATINGCOLUMN
    SUPPORTS_SMALLFLOATINGCOLUMN = _hk_classes.hk_connection_SUPPORTS_SMALLFLOATINGCOLUMN
    SUPPORTS_PROPRIETARYCOLUMN = _hk_classes.hk_connection_SUPPORTS_PROPRIETARYCOLUMN
    SUPPORTS_SQL = _hk_classes.hk_connection_SUPPORTS_SQL
    SUPPORTS_TRANSACTIONS = _hk_classes.hk_connection_SUPPORTS_TRANSACTIONS
    SUPPORTS_REFERENTIALINTEGRITY = _hk_classes.hk_connection_SUPPORTS_REFERENTIALINTEGRITY
    SUPPORTS_NEW_DATABASE = _hk_classes.hk_connection_SUPPORTS_NEW_DATABASE
    SUPPORTS_DELETE_DATABASE = _hk_classes.hk_connection_SUPPORTS_DELETE_DATABASE
    SUPPORTS_NEW_TABLE = _hk_classes.hk_connection_SUPPORTS_NEW_TABLE
    SUPPORTS_ALTER_TABLE = _hk_classes.hk_connection_SUPPORTS_ALTER_TABLE
    SUPPORTS_DELETE_TABLE = _hk_classes.hk_connection_SUPPORTS_DELETE_TABLE
    SUPPORTS_CREATE_INDEX = _hk_classes.hk_connection_SUPPORTS_CREATE_INDEX
    SUPPORTS_DELETE_INDEX = _hk_classes.hk_connection_SUPPORTS_DELETE_INDEX
    SUPPORTS_CHANGE_PASSWORD = _hk_classes.hk_connection_SUPPORTS_CHANGE_PASSWORD
    SUPPORTS_RENAME_DATABASE = _hk_classes.hk_connection_SUPPORTS_RENAME_DATABASE
    SUPPORTS_RENAME_TABLE = _hk_classes.hk_connection_SUPPORTS_RENAME_TABLE
    SUPPORTS_VIEWS = _hk_classes.hk_connection_SUPPORTS_VIEWS
    SUPPORTS_NEW_VIEW = _hk_classes.hk_connection_SUPPORTS_NEW_VIEW
    SUPPORTS_ALTER_VIEW = _hk_classes.hk_connection_SUPPORTS_ALTER_VIEW
    SUPPORTS_DELETE_VIEW = _hk_classes.hk_connection_SUPPORTS_DELETE_VIEW
    SUPPORTS_ADD_COLUMN = _hk_classes.hk_connection_SUPPORTS_ADD_COLUMN
    SUPPORTS_DELETE_COLUMN = _hk_classes.hk_connection_SUPPORTS_DELETE_COLUMN
    SUPPORTS_CHANGE_COLUMNTYPE = _hk_classes.hk_connection_SUPPORTS_CHANGE_COLUMNTYPE
    SUPPORTS_CHANGE_COLUMNNAME = _hk_classes.hk_connection_SUPPORTS_CHANGE_COLUMNNAME
    SUPPORTS_ALTER_PRIMARY_KEY = _hk_classes.hk_connection_SUPPORTS_ALTER_PRIMARY_KEY
    SUPPORTS_ALTER_NOT_NULL = _hk_classes.hk_connection_SUPPORTS_ALTER_NOT_NULL
    SUPPORTS_SQL_GROUP_BY = _hk_classes.hk_connection_SUPPORTS_SQL_GROUP_BY
    SUPPORTS_SQL_ORDER_BY = _hk_classes.hk_connection_SUPPORTS_SQL_ORDER_BY
    SUPPORTS_SQL_HAVING = _hk_classes.hk_connection_SUPPORTS_SQL_HAVING
    SUPPORTS_SQL_WHERE = _hk_classes.hk_connection_SUPPORTS_SQL_WHERE
    SUPPORTS_SQL_ALIAS = _hk_classes.hk_connection_SUPPORTS_SQL_ALIAS
    SUPPORTS_SQL_SUBSELECTS = _hk_classes.hk_connection_SUPPORTS_SQL_SUBSELECTS
    SUPPORTS_SQL_JOINS = _hk_classes.hk_connection_SUPPORTS_SQL_JOINS
    SUPPORTS_SQL_UNION = _hk_classes.hk_connection_SUPPORTS_SQL_UNION
    SUPPORTS_LOCAL_FILEFORMAT = _hk_classes.hk_connection_SUPPORTS_LOCAL_FILEFORMAT
    SUPPORTS_NONALPHANUM_FIELDNAMES = _hk_classes.hk_connection_SUPPORTS_NONALPHANUM_FIELDNAMES
    SUPPORTS_NONASCII_FIELDNAMES = _hk_classes.hk_connection_SUPPORTS_NONASCII_FIELDNAMES
    SUPPORTS_SPACE_FIELDNAMES = _hk_classes.hk_connection_SUPPORTS_SPACE_FIELDNAMES
    def server_supports(*args): return _hk_classes.hk_connection_server_supports(*args)
    NEEDS_LOGIN = _hk_classes.hk_connection_NEEDS_LOGIN
    NEEDS_HOST = _hk_classes.hk_connection_NEEDS_HOST
    NEEDS_USERNAME = _hk_classes.hk_connection_NEEDS_USERNAME
    NEEDS_PASSWORD = _hk_classes.hk_connection_NEEDS_PASSWORD
    NEEDS_SQLDELIMITER = _hk_classes.hk_connection_NEEDS_SQLDELIMITER
    NEEDS_PORT = _hk_classes.hk_connection_NEEDS_PORT
    NEEDS_DATABASENAME = _hk_classes.hk_connection_NEEDS_DATABASENAME
    NEEDS_BOOLEANEMULATION = _hk_classes.hk_connection_NEEDS_BOOLEANEMULATION
    NEEDS_NULL_TERMINATED_SQL = _hk_classes.hk_connection_NEEDS_NULL_TERMINATED_SQL
    NEEDS_TABLE_ALIAS_AS = _hk_classes.hk_connection_NEEDS_TABLE_ALIAS_AS
    def server_needs(*args): return _hk_classes.hk_connection_server_needs(*args)
    def set_newpassword(*args): return _hk_classes.hk_connection_set_newpassword(*args)
    def show_newpassworddialog(*args): return _hk_classes.hk_connection_show_newpassworddialog(*args)
    def show_passworddialog(*args): return _hk_classes.hk_connection_show_passworddialog(*args)
    def drivername(*args): return _hk_classes.hk_connection_drivername(*args)
    def set_booleanemulation(*args): return _hk_classes.hk_connection_set_booleanemulation(*args)
    def booleanemulation(*args): return _hk_classes.hk_connection_booleanemulation(*args)
    def copy_database(*args): return _hk_classes.hk_connection_copy_database(*args)
    def last_servermessage(*args): return _hk_classes.hk_connection_last_servermessage(*args)
    def databasepath(*args): return _hk_classes.hk_connection_databasepath(*args)
    def mimetype(*args): return _hk_classes.hk_connection_mimetype(*args)
    def maxfieldnamesize(*args): return _hk_classes.hk_connection_maxfieldnamesize(*args)

class hk_connectionPtr(hk_connection):
    def __init__(self, this):
        _swig_setattr(self, hk_connection, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_connection, 'thisown', 0)
        _swig_setattr(self, hk_connection,self.__class__,hk_connection)
_hk_classes.hk_connection_swigregister(hk_connectionPtr)

hk_connection_check_capabilities = _hk_classes.hk_connection_check_capabilities

class hk_drivermanager(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_drivermanager, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_drivermanager, name)
    def __repr__(self):
        return "<C hk_drivermanager instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_drivermanager, 'this', _hk_classes.new_hk_drivermanager(*args))
        _swig_setattr(self, hk_drivermanager, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_drivermanager):
        try:
            if self.thisown: destroy(self)
        except: pass
    def driverlist(*args): return _hk_classes.hk_drivermanager_driverlist(*args)
    def new_connection(*args): return _hk_classes.hk_drivermanager_new_connection(*args)
    def find_existing_connection(*args): return _hk_classes.hk_drivermanager_find_existing_connection(*args)
    __swig_getmethods__["show_driverselectdialog"] = lambda x: _hk_classes.hk_drivermanager_show_driverselectdialog
    if _newclass:show_driverselectdialog = staticmethod(_hk_classes.hk_drivermanager_show_driverselectdialog)
    __swig_getmethods__["set_path"] = lambda x: _hk_classes.hk_drivermanager_set_path
    if _newclass:set_path = staticmethod(_hk_classes.hk_drivermanager_set_path)
    __swig_getmethods__["path"] = lambda x: _hk_classes.hk_drivermanager_path
    if _newclass:path = staticmethod(_hk_classes.hk_drivermanager_path)

class hk_drivermanagerPtr(hk_drivermanager):
    def __init__(self, this):
        _swig_setattr(self, hk_drivermanager, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_drivermanager, 'thisown', 0)
        _swig_setattr(self, hk_drivermanager,self.__class__,hk_drivermanager)
_hk_classes.hk_drivermanager_swigregister(hk_drivermanagerPtr)

hk_drivermanager_show_driverselectdialog = _hk_classes.hk_drivermanager_show_driverselectdialog

hk_drivermanager_set_path = _hk_classes.hk_drivermanager_set_path

hk_drivermanager_path = _hk_classes.hk_drivermanager_path

class hk_database(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_database, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_database, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_database instance at %s>" % (self.this,)
    def tablelist(*args): return _hk_classes.hk_database_tablelist(*args)
    def querylist(*args): return _hk_classes.hk_database_querylist(*args)
    def formlist(*args): return _hk_classes.hk_database_formlist(*args)
    def reportlist(*args): return _hk_classes.hk_database_reportlist(*args)
    def viewlist(*args): return _hk_classes.hk_database_viewlist(*args)
    def tableviewlist(*args): return _hk_classes.hk_database_tableviewlist(*args)
    def new_table(*args): return _hk_classes.hk_database_new_table(*args)
    def new_resultquery(*args): return _hk_classes.hk_database_new_resultquery(*args)
    def new_view(*args): return _hk_classes.hk_database_new_view(*args)
    def new_actionquery(*args): return _hk_classes.hk_database_new_actionquery(*args)
    def load_datasource(*args): return _hk_classes.hk_database_load_datasource(*args)
    def delete_table(*args): return _hk_classes.hk_database_delete_table(*args)
    def delete_view(*args): return _hk_classes.hk_database_delete_view(*args)
    def table_exists(*args): return _hk_classes.hk_database_table_exists(*args)
    def query_exists(*args): return _hk_classes.hk_database_query_exists(*args)
    def view_exists(*args): return _hk_classes.hk_database_view_exists(*args)
    def form_exists(*args): return _hk_classes.hk_database_form_exists(*args)
    def report_exists(*args): return _hk_classes.hk_database_report_exists(*args)
    def load(*args): return _hk_classes.hk_database_load(*args)
    def save(*args): return _hk_classes.hk_database_save(*args)
    def connection(*args): return _hk_classes.hk_database_connection(*args)
    def new_formvisible(*args): return _hk_classes.hk_database_new_formvisible(*args)
    def new_dialogformvisible(*args): return _hk_classes.hk_database_new_dialogformvisible(*args)
    def new_reportvisible(*args): return _hk_classes.hk_database_new_reportvisible(*args)
    def new_tablevisible(*args): return _hk_classes.hk_database_new_tablevisible(*args)
    def new_queryvisible(*args): return _hk_classes.hk_database_new_queryvisible(*args)
    central = _hk_classes.hk_database_central
    local = _hk_classes.hk_database_local
    def set_storagemode(*args): return _hk_classes.hk_database_set_storagemode(*args)
    def storagemode(*args): return _hk_classes.hk_database_storagemode(*args)
    def loadmode(*args): return _hk_classes.hk_database_loadmode(*args)
    def create_centralstoragetable(*args): return _hk_classes.hk_database_create_centralstoragetable(*args)
    def has_centralstoragetable(*args): return _hk_classes.hk_database_has_centralstoragetable(*args)
    def load_local(*args): return _hk_classes.hk_database_load_local(*args)
    def load_central(*args): return _hk_classes.hk_database_load_central(*args)
    def save_local(*args): return _hk_classes.hk_database_save_local(*args)
    def save_central(*args): return _hk_classes.hk_database_save_central(*args)
    def delete_localfile(*args): return _hk_classes.hk_database_delete_localfile(*args)
    def delete_centralfile(*args): return _hk_classes.hk_database_delete_centralfile(*args)
    def filelist(*args): return _hk_classes.hk_database_filelist(*args)
    def local_filelist(*args): return _hk_classes.hk_database_local_filelist(*args)
    def central_filelist(*args): return _hk_classes.hk_database_central_filelist(*args)
    def database_path(*args): return _hk_classes.hk_database_database_path(*args)
    def save_configuration(*args): return _hk_classes.hk_database_save_configuration(*args)
    def disable(*args): return _hk_classes.hk_database_disable(*args)
    def existing_presentation(*args): return _hk_classes.hk_database_existing_presentation(*args)
    def existing_form(*args): return _hk_classes.hk_database_existing_form(*args)
    def existing_report(*args): return _hk_classes.hk_database_existing_report(*args)
    def set_name(*args): return _hk_classes.hk_database_set_name(*args)
    def name(*args): return _hk_classes.hk_database_name(*args)
    def copy_table(*args): return _hk_classes.hk_database_copy_table(*args)
    def copy_view(*args): return _hk_classes.hk_database_copy_view(*args)
    def rename_table(*args): return _hk_classes.hk_database_rename_table(*args)
    def rename_file(*args): return _hk_classes.hk_database_rename_file(*args)
    def set_databasecharset(*args): return _hk_classes.hk_database_set_databasecharset(*args)
    def databasecharset(*args): return _hk_classes.hk_database_databasecharset(*args)
    def set_automatic_data_update(*args): return _hk_classes.hk_database_set_automatic_data_update(*args)
    def is_automatic_data_update(*args): return _hk_classes.hk_database_is_automatic_data_update(*args)

class hk_databasePtr(hk_database):
    def __init__(self, this):
        _swig_setattr(self, hk_database, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_database, 'thisown', 0)
        _swig_setattr(self, hk_database,self.__class__,hk_database)
_hk_classes.hk_database_swigregister(hk_databasePtr)

class hk_data(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_data, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_data, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_data instance at %s>" % (self.this,)
    def name(*args): return _hk_classes.hk_data_name(*args)
    def set_name(*args): return _hk_classes.hk_data_set_name(*args)
    ds_table = _hk_classes.hk_data_ds_table
    ds_query = _hk_classes.hk_data_ds_query
    ds_actionquery = _hk_classes.hk_data_ds_actionquery
    ds_view = _hk_classes.hk_data_ds_view
    ds_unknown = _hk_classes.hk_data_ds_unknown
    def type(*args): return _hk_classes.hk_data_type(*args)
    def identifierdelimiter(*args): return _hk_classes.hk_data_identifierdelimiter(*args)
    def textdelimiter(*args): return _hk_classes.hk_data_textdelimiter(*args)
    __swig_getmethods__["print_sqlstatements"] = lambda x: _hk_classes.hk_data_print_sqlstatements
    if _newclass:print_sqlstatements = staticmethod(_hk_classes.hk_data_print_sqlstatements)
    __swig_getmethods__["set_print_sqlstatements"] = lambda x: _hk_classes.hk_data_set_print_sqlstatements
    if _newclass:set_print_sqlstatements = staticmethod(_hk_classes.hk_data_set_print_sqlstatements)

class hk_dataPtr(hk_data):
    def __init__(self, this):
        _swig_setattr(self, hk_data, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_data, 'thisown', 0)
        _swig_setattr(self, hk_data,self.__class__,hk_data)
_hk_classes.hk_data_swigregister(hk_dataPtr)
cvar = _hk_classes.cvar
defaulttextdelimiter = cvar.defaulttextdelimiter
defaultidentifierdelimiter = cvar.defaultidentifierdelimiter

hk_data_print_sqlstatements = _hk_classes.hk_data_print_sqlstatements

hk_data_set_print_sqlstatements = _hk_classes.hk_data_set_print_sqlstatements

class hk_datasource(hk_data):
    __swig_setmethods__ = {}
    for _s in [hk_data]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_datasource, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_data]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_datasource, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_datasource instance at %s>" % (self.this,)
    def name(*args): return _hk_classes.hk_datasource_name(*args)
    def set_name(*args): return _hk_classes.hk_datasource_set_name(*args)
    def set_sql(*args): return _hk_classes.hk_datasource_set_sql(*args)
    def sql(*args): return _hk_classes.hk_datasource_sql(*args)
    def backendsql(*args): return _hk_classes.hk_datasource_backendsql(*args)
    def is_rawsql(*args): return _hk_classes.hk_datasource_is_rawsql(*args)
    def goto_row(*args): return _hk_classes.hk_datasource_goto_row(*args)
    def goto_first(*args): return _hk_classes.hk_datasource_goto_first(*args)
    def goto_last(*args): return _hk_classes.hk_datasource_goto_last(*args)
    def goto_next(*args): return _hk_classes.hk_datasource_goto_next(*args)
    def goto_previous(*args): return _hk_classes.hk_datasource_goto_previous(*args)
    def row_position(*args): return _hk_classes.hk_datasource_row_position(*args)
    def max_rows(*args): return _hk_classes.hk_datasource_max_rows(*args)
    def enable(*args): return _hk_classes.hk_datasource_enable(*args)
    def disable(*args): return _hk_classes.hk_datasource_disable(*args)
    def set_enabled(*args): return _hk_classes.hk_datasource_set_enabled(*args)
    def is_enabled(*args): return _hk_classes.hk_datasource_is_enabled(*args)
    def column_by_name(*args): return _hk_classes.hk_datasource_column_by_name(*args)
    def store_changed_data(*args): return _hk_classes.hk_datasource_store_changed_data(*args)
    def set_automatic_data_update(*args): return _hk_classes.hk_datasource_set_automatic_data_update(*args)
    def is_automatic_data_update(*args): return _hk_classes.hk_datasource_is_automatic_data_update(*args)
    mode_normal = _hk_classes.hk_datasource_mode_normal
    mode_createtable = _hk_classes.hk_datasource_mode_createtable
    mode_altertable = _hk_classes.hk_datasource_mode_altertable
    mode_disabled = _hk_classes.hk_datasource_mode_disabled
    mode_insertrow = _hk_classes.hk_datasource_mode_insertrow
    mode_deleterow = _hk_classes.hk_datasource_mode_deleterow
    mode_unknown = _hk_classes.hk_datasource_mode_unknown
    def mode(*args): return _hk_classes.hk_datasource_mode(*args)
    def setmode_insertrow(*args): return _hk_classes.hk_datasource_setmode_insertrow(*args)
    def delete_actualrow(*args): return _hk_classes.hk_datasource_delete_actualrow(*args)
    def delete_rows(*args): return _hk_classes.hk_datasource_delete_rows(*args)
    def setmode_normal(*args): return _hk_classes.hk_datasource_setmode_normal(*args)
    def setmode_createtable(*args): return _hk_classes.hk_datasource_setmode_createtable(*args)
    def setmode_altertable(*args): return _hk_classes.hk_datasource_setmode_altertable(*args)
    def new_column(*args): return _hk_classes.hk_datasource_new_column(*args)
    def alter_column(*args): return _hk_classes.hk_datasource_alter_column(*args)
    def delete_column(*args): return _hk_classes.hk_datasource_delete_column(*args)
    def create_table_now(*args): return _hk_classes.hk_datasource_create_table_now(*args)
    def alter_table_now(*args): return _hk_classes.hk_datasource_alter_table_now(*args)
    standard = _hk_classes.hk_datasource_standard
    batchread = _hk_classes.hk_datasource_batchread
    batchwrite = _hk_classes.hk_datasource_batchwrite
    def set_accessmode(*args): return _hk_classes.hk_datasource_set_accessmode(*args)
    def accessmode(*args): return _hk_classes.hk_datasource_accessmode(*args)
    def is_readonly(*args): return _hk_classes.hk_datasource_is_readonly(*args)
    def set_readonly(*args): return _hk_classes.hk_datasource_set_readonly(*args)
    def drop_index(*args): return _hk_classes.hk_datasource_drop_index(*args)
    def create_index(*args): return _hk_classes.hk_datasource_create_index(*args)
    def alter_index(*args): return _hk_classes.hk_datasource_alter_index(*args)
    def database(*args): return _hk_classes.hk_datasource_database(*args)
    def set_ignore_changed_data(*args): return _hk_classes.hk_datasource_set_ignore_changed_data(*args)
    def ignore_changed_data(*args): return _hk_classes.hk_datasource_ignore_changed_data(*args)
    def set_filter(*args): return _hk_classes.hk_datasource_set_filter(*args)
    def filter(*args): return _hk_classes.hk_datasource_filter(*args)
    def set_temporaryfilter(*args): return _hk_classes.hk_datasource_set_temporaryfilter(*args)
    def temporaryfilter(*args): return _hk_classes.hk_datasource_temporaryfilter(*args)
    def set_use_temporaryfilter(*args): return _hk_classes.hk_datasource_set_use_temporaryfilter(*args)
    def use_temporaryfilter(*args): return _hk_classes.hk_datasource_use_temporaryfilter(*args)
    def clear_filter(*args): return _hk_classes.hk_datasource_clear_filter(*args)
    def set_sorting(*args): return _hk_classes.hk_datasource_set_sorting(*args)
    def sorting(*args): return _hk_classes.hk_datasource_sorting(*args)
    def set_temporarysorting(*args): return _hk_classes.hk_datasource_set_temporarysorting(*args)
    def temporarysorting(*args): return _hk_classes.hk_datasource_temporarysorting(*args)
    def set_use_temporarysorting(*args): return _hk_classes.hk_datasource_set_use_temporarysorting(*args)
    def use_temporarysorting(*args): return _hk_classes.hk_datasource_use_temporarysorting(*args)
    def clear_sorting(*args): return _hk_classes.hk_datasource_clear_sorting(*args)
    def datasource_used(*args): return _hk_classes.hk_datasource_datasource_used(*args)
    def referenceslist(*args): return _hk_classes.hk_datasource_referenceslist(*args)
    def add_reference(*args): return _hk_classes.hk_datasource_add_reference(*args)
    def drop_reference(*args): return _hk_classes.hk_datasource_drop_reference(*args)
    def has_changed(*args): return _hk_classes.hk_datasource_has_changed(*args)
    def dateformat(*args): return _hk_classes.hk_datasource_dateformat(*args)
    def datetimeformat(*args): return _hk_classes.hk_datasource_datetimeformat(*args)
    def timeformat(*args): return _hk_classes.hk_datasource_timeformat(*args)
    depending_nohandle = _hk_classes.hk_datasource_depending_nohandle
    depending_standard = _hk_classes.hk_datasource_depending_standard
    depending_change = _hk_classes.hk_datasource_depending_change
    depending_delete = _hk_classes.hk_datasource_depending_delete
    depending_changedelete = _hk_classes.hk_datasource_depending_changedelete
    def dependingmode(*args): return _hk_classes.hk_datasource_dependingmode(*args)
    def set_depending_on(*args): return _hk_classes.hk_datasource_set_depending_on(*args)
    def depending_on(*args): return _hk_classes.hk_datasource_depending_on(*args)
    def set_depending_on_is_left_join(*args): return _hk_classes.hk_datasource_set_depending_on_is_left_join(*args)
    def depending_on_is_left_join(*args): return _hk_classes.hk_datasource_depending_on_is_left_join(*args)
    def depending_on_react_on_data_changes(*args): return _hk_classes.hk_datasource_depending_on_react_on_data_changes(*args)
    def depending_on_thisfields(*args): return _hk_classes.hk_datasource_depending_on_thisfields(*args)
    def depending_on_masterfields(*args): return _hk_classes.hk_datasource_depending_on_masterfields(*args)
    def set_depending_on_presentationdatasource(*args): return _hk_classes.hk_datasource_set_depending_on_presentationdatasource(*args)
    def depending_on_presentationdatasource(*args): return _hk_classes.hk_datasource_depending_on_presentationdatasource(*args)
    def add_depending_fields(*args): return _hk_classes.hk_datasource_add_depending_fields(*args)
    def clear_depending_fields(*args): return _hk_classes.hk_datasource_clear_depending_fields(*args)
    def columnlist(*args): return _hk_classes.hk_datasource_columnlist(*args)
    def indexlist(*args): return _hk_classes.hk_datasource_indexlist(*args)
    def show_currentrow(*args): return _hk_classes.hk_datasource_show_currentrow(*args)
    def indices(*args): return _hk_classes.hk_datasource_indices(*args)
    def __del__(self, destroy=_hk_classes.delete_hk_datasource):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_datasourcePtr(hk_datasource):
    def __init__(self, this):
        _swig_setattr(self, hk_datasource, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_datasource, 'thisown', 0)
        _swig_setattr(self, hk_datasource,self.__class__,hk_datasource)
_hk_classes.hk_datasource_swigregister(hk_datasourcePtr)

class hk_column(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_column, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_column, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_column instance at %s>" % (self.this,)
    def name(*args): return _hk_classes.hk_column_name(*args)
    def set_name(*args): return _hk_classes.hk_column_set_name(*args)
    def is_readonly(*args): return _hk_classes.hk_column_is_readonly(*args)
    def set_readonly(*args): return _hk_classes.hk_column_set_readonly(*args)
    textcolumn = _hk_classes.hk_column_textcolumn
    auto_inccolumn = _hk_classes.hk_column_auto_inccolumn
    smallintegercolumn = _hk_classes.hk_column_smallintegercolumn
    integercolumn = _hk_classes.hk_column_integercolumn
    smallfloatingcolumn = _hk_classes.hk_column_smallfloatingcolumn
    floatingcolumn = _hk_classes.hk_column_floatingcolumn
    datecolumn = _hk_classes.hk_column_datecolumn
    datetimecolumn = _hk_classes.hk_column_datetimecolumn
    timecolumn = _hk_classes.hk_column_timecolumn
    timestampcolumn = _hk_classes.hk_column_timestampcolumn
    binarycolumn = _hk_classes.hk_column_binarycolumn
    memocolumn = _hk_classes.hk_column_memocolumn
    boolcolumn = _hk_classes.hk_column_boolcolumn
    othercolumn = _hk_classes.hk_column_othercolumn
    def columntype(*args): return _hk_classes.hk_column_columntype(*args)
    def set_columntype(*args): return _hk_classes.hk_column_set_columntype(*args)
    def size(*args): return _hk_classes.hk_column_size(*args)
    def set_size(*args): return _hk_classes.hk_column_set_size(*args)
    def columntype_name(*args): return _hk_classes.hk_column_columntype_name(*args)
    def is_primary(*args): return _hk_classes.hk_column_is_primary(*args)
    def set_primary(*args): return _hk_classes.hk_column_set_primary(*args)
    def is_notnull(*args): return _hk_classes.hk_column_is_notnull(*args)
    def get_delimiter(*args): return _hk_classes.hk_column_get_delimiter(*args)
    def set_notnull(*args): return _hk_classes.hk_column_set_notnull(*args)
    def has_changed(*args): return _hk_classes.hk_column_has_changed(*args)
    def asstring(*args): return _hk_classes.hk_column_asstring(*args)
    def set_asstring(*args): return _hk_classes.hk_column_set_asstring(*args)
    def transformed_asstring(*args): return _hk_classes.hk_column_transformed_asstring(*args)
    def transformed_asstring_at(*args): return _hk_classes.hk_column_transformed_asstring_at(*args)
    def changed_data_asstring(*args): return _hk_classes.hk_column_changed_data_asstring(*args)
    def set_asbinary(*args): return _hk_classes.hk_column_set_asbinary(*args)
    def asbinary(*args): return _hk_classes.hk_column_asbinary(*args)
    def asbinary_at(*args): return _hk_classes.hk_column_asbinary_at(*args)
    def set_asbool(*args): return _hk_classes.hk_column_set_asbool(*args)
    def asbool(*args): return _hk_classes.hk_column_asbool(*args)
    def asbool_at(*args): return _hk_classes.hk_column_asbool_at(*args)
    def changed_data_asbool(*args): return _hk_classes.hk_column_changed_data_asbool(*args)
    def set_asinteger(*args): return _hk_classes.hk_column_set_asinteger(*args)
    def asinteger(*args): return _hk_classes.hk_column_asinteger(*args)
    def asinteger_at(*args): return _hk_classes.hk_column_asinteger_at(*args)
    def changed_data_asinteger(*args): return _hk_classes.hk_column_changed_data_asinteger(*args)
    def set_asdouble(*args): return _hk_classes.hk_column_set_asdouble(*args)
    def asdouble(*args): return _hk_classes.hk_column_asdouble(*args)
    def asdouble_at(*args): return _hk_classes.hk_column_asdouble_at(*args)
    def changed_data_asdouble(*args): return _hk_classes.hk_column_changed_data_asdouble(*args)
    def fieldnumber(*args): return _hk_classes.hk_column_fieldnumber(*args)
    def asstring_at(*args): return _hk_classes.hk_column_asstring_at(*args)
    def is_nullvalue(*args): return _hk_classes.hk_column_is_nullvalue(*args)
    def changed_data_is_nullvalue(*args): return _hk_classes.hk_column_changed_data_is_nullvalue(*args)
    def is_nullvalue_at(*args): return _hk_classes.hk_column_is_nullvalue_at(*args)
    def set_asnullvalue(*args): return _hk_classes.hk_column_set_asnullvalue(*args)
    def count(*args): return _hk_classes.hk_column_count(*args)
    def sum(*args): return _hk_classes.hk_column_sum(*args)
    def find(*args): return _hk_classes.hk_column_find(*args)
    def is_findstring(*args): return _hk_classes.hk_column_is_findstring(*args)
    def changed_data(*args): return _hk_classes.hk_column_changed_data(*args)
    def transformed_changed_data(*args): return _hk_classes.hk_column_transformed_changed_data(*args)
    def reset_changed_data(*args): return _hk_classes.hk_column_reset_changed_data(*args)
    def set_boolvalues(*args): return _hk_classes.hk_column_set_boolvalues(*args)
    def bool_truevalue(*args): return _hk_classes.hk_column_bool_truevalue(*args)
    def bool_falsevalue(*args): return _hk_classes.hk_column_bool_falsevalue(*args)
    def set_dateformat(*args): return _hk_classes.hk_column_set_dateformat(*args)
    def dateformat(*args): return _hk_classes.hk_column_dateformat(*args)
    def set_timeformat(*args): return _hk_classes.hk_column_set_timeformat(*args)
    def timeformat(*args): return _hk_classes.hk_column_timeformat(*args)
    def set_datetimeformat(*args): return _hk_classes.hk_column_set_datetimeformat(*args)
    def datetimeformat(*args): return _hk_classes.hk_column_datetimeformat(*args)
    def save_to_file(*args): return _hk_classes.hk_column_save_to_file(*args)
    def load_from_file(*args): return _hk_classes.hk_column_load_from_file(*args)
    def save_columndefinition(*args): return _hk_classes.hk_column_save_columndefinition(*args)
    def load_columndefinition(*args): return _hk_classes.hk_column_load_columndefinition(*args)
    def datasource(*args): return _hk_classes.hk_column_datasource(*args)
    def set_allow_autoincwrite(*args): return _hk_classes.hk_column_set_allow_autoincwrite(*args)
    def allow_autoincwrite(*args): return _hk_classes.hk_column_allow_autoincwrite(*args)
    def in_definitionmode(*args): return _hk_classes.hk_column_in_definitionmode(*args)
    def curval_asstring(*args): return _hk_classes.hk_column_curval_asstring(*args)
    def curval_asdouble(*args): return _hk_classes.hk_column_curval_asdouble(*args)
    def curval_asinteger(*args): return _hk_classes.hk_column_curval_asinteger(*args)
    def curval_asbool(*args): return _hk_classes.hk_column_curval_asbool(*args)
    def set_definitionmode(*args): return _hk_classes.hk_column_set_definitionmode(*args)
    def tableorigin(*args): return _hk_classes.hk_column_tableorigin(*args)

class hk_columnPtr(hk_column):
    def __init__(self, this):
        _swig_setattr(self, hk_column, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_column, 'thisown', 0)
        _swig_setattr(self, hk_column,self.__class__,hk_column)
_hk_classes.hk_column_swigregister(hk_columnPtr)

class hk_visible(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_visible, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_visible, name)
    def __repr__(self):
        return "<C hk_visible instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_visible, 'this', _hk_classes.new_hk_visible(*args))
        _swig_setattr(self, hk_visible, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_visible):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_x(*args): return _hk_classes.hk_visible_set_x(*args)
    def set_y(*args): return _hk_classes.hk_visible_set_y(*args)
    def set_width(*args): return _hk_classes.hk_visible_set_width(*args)
    def set_height(*args): return _hk_classes.hk_visible_set_height(*args)
    def x(*args): return _hk_classes.hk_visible_x(*args)
    def y(*args): return _hk_classes.hk_visible_y(*args)
    def width(*args): return _hk_classes.hk_visible_width(*args)
    def height(*args): return _hk_classes.hk_visible_height(*args)
    def set_size(*args): return _hk_classes.hk_visible_set_size(*args)
    def set_position(*args): return _hk_classes.hk_visible_set_position(*args)
    def set_label(*args): return _hk_classes.hk_visible_set_label(*args)
    def label(*args): return _hk_classes.hk_visible_label(*args)
    def set_identifier(*args): return _hk_classes.hk_visible_set_identifier(*args)
    def identifier(*args): return _hk_classes.hk_visible_identifier(*args)
    textlabel = _hk_classes.hk_visible_textlabel
    button = _hk_classes.hk_visible_button
    rowselector = _hk_classes.hk_visible_rowselector
    boolean = _hk_classes.hk_visible_boolean
    lineedit = _hk_classes.hk_visible_lineedit
    memo = _hk_classes.hk_visible_memo
    combobox = _hk_classes.hk_visible_combobox
    grid = _hk_classes.hk_visible_grid
    form = _hk_classes.hk_visible_form
    report = _hk_classes.hk_visible_report
    reportsection = _hk_classes.hk_visible_reportsection
    reportdata = _hk_classes.hk_visible_reportdata
    query = _hk_classes.hk_visible_query
    subform = _hk_classes.hk_visible_subform
    image = _hk_classes.hk_visible_image
    date = _hk_classes.hk_visible_date
    tabvisible = _hk_classes.hk_visible_tabvisible
    other = _hk_classes.hk_visible_other
    def type(*args): return _hk_classes.hk_visible_type(*args)
    def set_font(*args): return _hk_classes.hk_visible_set_font(*args)
    def font(*args): return _hk_classes.hk_visible_font(*args)
    def set_foregroundcolour(*args): return _hk_classes.hk_visible_set_foregroundcolour(*args)
    def foregroundcolour(*args): return _hk_classes.hk_visible_foregroundcolour(*args)
    def set_backgroundcolour(*args): return _hk_classes.hk_visible_set_backgroundcolour(*args)
    def backgroundcolour(*args): return _hk_classes.hk_visible_backgroundcolour(*args)
    def set_neverregisterchange(*args): return _hk_classes.hk_visible_set_neverregisterchange(*args)
    def neverregisterchange(*args): return _hk_classes.hk_visible_neverregisterchange(*args)
    def savedata(*args): return _hk_classes.hk_visible_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_visible_loaddata(*args)
    def presentationnumber(*args): return _hk_classes.hk_visible_presentationnumber(*args)
    def set_datetimeformat(*args): return _hk_classes.hk_visible_set_datetimeformat(*args)
    def datetimeformat(*args): return _hk_classes.hk_visible_datetimeformat(*args)
    def dateformat(*args): return _hk_classes.hk_visible_dateformat(*args)
    def timeformat(*args): return _hk_classes.hk_visible_timeformat(*args)
    def set_counts_as(*args): return _hk_classes.hk_visible_set_counts_as(*args)
    def counts_as(*args): return _hk_classes.hk_visible_counts_as(*args)
    def replace(*args): return _hk_classes.hk_visible_replace(*args)
    def presentation(*args): return _hk_classes.hk_visible_presentation(*args)
    def set_load_presentationnumber(*args): return _hk_classes.hk_visible_set_load_presentationnumber(*args)
    def load_presentationnumber(*args): return _hk_classes.hk_visible_load_presentationnumber(*args)
    alignleft = _hk_classes.hk_visible_alignleft
    alignright = _hk_classes.hk_visible_alignright
    aligncenter = _hk_classes.hk_visible_aligncenter
    aligndefault = _hk_classes.hk_visible_aligndefault
    def set_alignment(*args): return _hk_classes.hk_visible_set_alignment(*args)
    def alignment(*args): return _hk_classes.hk_visible_alignment(*args)
    __swig_getmethods__["set_defaulttextalignment"] = lambda x: _hk_classes.hk_visible_set_defaulttextalignment
    if _newclass:set_defaulttextalignment = staticmethod(_hk_classes.hk_visible_set_defaulttextalignment)
    __swig_getmethods__["defaulttextalignment"] = lambda x: _hk_classes.hk_visible_defaulttextalignment
    if _newclass:defaulttextalignment = staticmethod(_hk_classes.hk_visible_defaulttextalignment)
    __swig_getmethods__["set_defaultnumberalignment"] = lambda x: _hk_classes.hk_visible_set_defaultnumberalignment
    if _newclass:set_defaultnumberalignment = staticmethod(_hk_classes.hk_visible_set_defaultnumberalignment)
    __swig_getmethods__["defaultnumberalignment"] = lambda x: _hk_classes.hk_visible_defaultnumberalignment
    if _newclass:defaultnumberalignment = staticmethod(_hk_classes.hk_visible_defaultnumberalignment)
    __swig_getmethods__["set_open_maximized_windows"] = lambda x: _hk_classes.hk_visible_set_open_maximized_windows
    if _newclass:set_open_maximized_windows = staticmethod(_hk_classes.hk_visible_set_open_maximized_windows)
    __swig_getmethods__["open_maximized_windows"] = lambda x: _hk_classes.hk_visible_open_maximized_windows
    if _newclass:open_maximized_windows = staticmethod(_hk_classes.hk_visible_open_maximized_windows)
    def set_tooltip(*args): return _hk_classes.hk_visible_set_tooltip(*args)
    def tooltip(*args): return _hk_classes.hk_visible_tooltip(*args)
    def show_widget(*args): return _hk_classes.hk_visible_show_widget(*args)
    def hide_widget(*args): return _hk_classes.hk_visible_hide_widget(*args)
    def close_widget(*args): return _hk_classes.hk_visible_close_widget(*args)
    def lower_widget(*args): return _hk_classes.hk_visible_lower_widget(*args)
    def raise_widget(*args): return _hk_classes.hk_visible_raise_widget(*args)
    def set_on_click_action(*args): return _hk_classes.hk_visible_set_on_click_action(*args)
    def on_click_action(*args): return _hk_classes.hk_visible_on_click_action(*args)
    def set_on_doubleclick_action(*args): return _hk_classes.hk_visible_set_on_doubleclick_action(*args)
    def on_doubleclick_action(*args): return _hk_classes.hk_visible_on_doubleclick_action(*args)
    def set_on_open_action(*args): return _hk_classes.hk_visible_set_on_open_action(*args)
    def on_open_action(*args): return _hk_classes.hk_visible_on_open_action(*args)
    def set_on_close_action(*args): return _hk_classes.hk_visible_set_on_close_action(*args)
    def on_close_action(*args): return _hk_classes.hk_visible_on_close_action(*args)
    def set_on_getfocus_action(*args): return _hk_classes.hk_visible_set_on_getfocus_action(*args)
    def on_getfocus_action(*args): return _hk_classes.hk_visible_on_getfocus_action(*args)
    def set_on_loosefocus_action(*args): return _hk_classes.hk_visible_set_on_loosefocus_action(*args)
    def on_loosefocus_action(*args): return _hk_classes.hk_visible_on_loosefocus_action(*args)
    def set_on_key_action(*args): return _hk_classes.hk_visible_set_on_key_action(*args)
    def on_key_action(*args): return _hk_classes.hk_visible_on_key_action(*args)
    def set_key(*args): return _hk_classes.hk_visible_set_key(*args)
    def key(*args): return _hk_classes.hk_visible_key(*args)
    def set_enabled(*args): return _hk_classes.hk_visible_set_enabled(*args)
    def is_enabled(*args): return _hk_classes.hk_visible_is_enabled(*args)
    def action_on_click(*args): return _hk_classes.hk_visible_action_on_click(*args)
    def action_on_doubleclick(*args): return _hk_classes.hk_visible_action_on_doubleclick(*args)
    def action_on_close(*args): return _hk_classes.hk_visible_action_on_close(*args)
    def action_on_open(*args): return _hk_classes.hk_visible_action_on_open(*args)
    def action_on_getfocus(*args): return _hk_classes.hk_visible_action_on_getfocus(*args)
    def action_on_loosefocus(*args): return _hk_classes.hk_visible_action_on_loosefocus(*args)
    def action_on_key(*args): return _hk_classes.hk_visible_action_on_key(*args)
    def in_tabvisible(*args): return _hk_classes.hk_visible_in_tabvisible(*args)
    def set_buddylabel(*args): return _hk_classes.hk_visible_set_buddylabel(*args)
    def buddylabel(*args): return _hk_classes.hk_visible_buddylabel(*args)
    def set_movebuddylabel(*args): return _hk_classes.hk_visible_set_movebuddylabel(*args)
    def move_buddylabel(*args): return _hk_classes.hk_visible_move_buddylabel(*args)
    def tag_number(*args): return _hk_classes.hk_visible_tag_number(*args)
    def column_value(*args): return _hk_classes.hk_visible_column_value(*args)
    def tag_value(*args): return _hk_classes.hk_visible_tag_value(*args)
    def register_tag(*args): return _hk_classes.hk_visible_register_tag(*args)

class hk_visiblePtr(hk_visible):
    def __init__(self, this):
        _swig_setattr(self, hk_visible, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_visible, 'thisown', 0)
        _swig_setattr(self, hk_visible,self.__class__,hk_visible)
_hk_classes.hk_visible_swigregister(hk_visiblePtr)

hk_visible_set_defaulttextalignment = _hk_classes.hk_visible_set_defaulttextalignment

hk_visible_defaulttextalignment = _hk_classes.hk_visible_defaulttextalignment

hk_visible_set_defaultnumberalignment = _hk_classes.hk_visible_set_defaultnumberalignment

hk_visible_defaultnumberalignment = _hk_classes.hk_visible_defaultnumberalignment

hk_visible_set_open_maximized_windows = _hk_classes.hk_visible_set_open_maximized_windows

hk_visible_open_maximized_windows = _hk_classes.hk_visible_open_maximized_windows


currentvisible = _hk_classes.currentvisible

cast_button = _hk_classes.cast_button

cast_dslineedit = _hk_classes.cast_dslineedit

cast_dsmemo = _hk_classes.cast_dsmemo

cast_dsgrid = _hk_classes.cast_dsgrid

cast_dscombobox = _hk_classes.cast_dscombobox

cast_dsboolean = _hk_classes.cast_dsboolean

cast_dsvisible = _hk_classes.cast_dsvisible

cast_subform = _hk_classes.cast_subform

cast_form = _hk_classes.cast_form

cast_dsimage = _hk_classes.cast_dsimage

cast_dsdate = _hk_classes.cast_dsdate

cast_report = _hk_classes.cast_report
class hk_dsvisible(hk_visible):
    __swig_setmethods__ = {}
    for _s in [hk_visible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsvisible, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_visible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsvisible, name)
    def __repr__(self):
        return "<C hk_dsvisible instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsvisible, 'this', _hk_classes.new_hk_dsvisible(*args))
        _swig_setattr(self, hk_dsvisible, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsvisible):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_datasource(*args): return _hk_classes.hk_dsvisible_set_datasource(*args)
    def set_presentationdatasource(*args): return _hk_classes.hk_dsvisible_set_presentationdatasource(*args)
    def presentationdatasource(*args): return _hk_classes.hk_dsvisible_presentationdatasource(*args)
    def datasource(*args): return _hk_classes.hk_dsvisible_datasource(*args)
    def savedata(*args): return _hk_classes.hk_dsvisible_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dsvisible_loaddata(*args)
    def set_readonly(*args): return _hk_classes.hk_dsvisible_set_readonly(*args)
    def is_readonly(*args): return _hk_classes.hk_dsvisible_is_readonly(*args)
    def set_before_row_change_action(*args): return _hk_classes.hk_dsvisible_set_before_row_change_action(*args)
    def before_row_change_action(*args): return _hk_classes.hk_dsvisible_before_row_change_action(*args)
    def set_after_row_change_action(*args): return _hk_classes.hk_dsvisible_set_after_row_change_action(*args)
    def after_row_change_action(*args): return _hk_classes.hk_dsvisible_after_row_change_action(*args)
    def set_before_update_action(*args): return _hk_classes.hk_dsvisible_set_before_update_action(*args)
    def before_update_action(*args): return _hk_classes.hk_dsvisible_before_update_action(*args)
    def set_after_update_action(*args): return _hk_classes.hk_dsvisible_set_after_update_action(*args)
    def after_update_action(*args): return _hk_classes.hk_dsvisible_after_update_action(*args)
    def set_before_delete_action(*args): return _hk_classes.hk_dsvisible_set_before_delete_action(*args)
    def before_delete_action(*args): return _hk_classes.hk_dsvisible_before_delete_action(*args)
    def set_after_delete_action(*args): return _hk_classes.hk_dsvisible_set_after_delete_action(*args)
    def after_delete_action(*args): return _hk_classes.hk_dsvisible_after_delete_action(*args)
    def set_before_insert_action(*args): return _hk_classes.hk_dsvisible_set_before_insert_action(*args)
    def before_insert_action(*args): return _hk_classes.hk_dsvisible_before_insert_action(*args)
    def set_after_insert_action(*args): return _hk_classes.hk_dsvisible_set_after_insert_action(*args)
    def after_insert_action(*args): return _hk_classes.hk_dsvisible_after_insert_action(*args)
    def tag_value(*args): return _hk_classes.hk_dsvisible_tag_value(*args)
    def action_before_store_changed_data(*args): return _hk_classes.hk_dsvisible_action_before_store_changed_data(*args)
    def action_after_store_changed_data(*args): return _hk_classes.hk_dsvisible_action_after_store_changed_data(*args)
    def action_before_row_change(*args): return _hk_classes.hk_dsvisible_action_before_row_change(*args)
    def action_after_row_change(*args): return _hk_classes.hk_dsvisible_action_after_row_change(*args)
    def action_before_delete(*args): return _hk_classes.hk_dsvisible_action_before_delete(*args)
    def action_after_delete(*args): return _hk_classes.hk_dsvisible_action_after_delete(*args)
    def action_before_insert(*args): return _hk_classes.hk_dsvisible_action_before_insert(*args)
    def action_after_insert(*args): return _hk_classes.hk_dsvisible_action_after_insert(*args)

class hk_dsvisiblePtr(hk_dsvisible):
    def __init__(self, this):
        _swig_setattr(self, hk_dsvisible, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsvisible, 'thisown', 0)
        _swig_setattr(self, hk_dsvisible,self.__class__,hk_dsvisible)
_hk_classes.hk_dsvisible_swigregister(hk_dsvisiblePtr)


currentdsvisible = _hk_classes.currentdsvisible

currentgrid = _hk_classes.currentgrid
class hk_dsdatavisible(hk_dsvisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsvisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsdatavisible, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsvisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsdatavisible, name)
    def __repr__(self):
        return "<C hk_dsdatavisible instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsdatavisible, 'this', _hk_classes.new_hk_dsdatavisible(*args))
        _swig_setattr(self, hk_dsdatavisible, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsdatavisible):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_columnname(*args): return _hk_classes.hk_dsdatavisible_set_columnname(*args)
    def column(*args): return _hk_classes.hk_dsdatavisible_column(*args)
    def columnname(*args): return _hk_classes.hk_dsdatavisible_columnname(*args)
    def set_datasource(*args): return _hk_classes.hk_dsdatavisible_set_datasource(*args)
    def datasource(*args): return _hk_classes.hk_dsdatavisible_datasource(*args)
    def savedata(*args): return _hk_classes.hk_dsdatavisible_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dsdatavisible_loaddata(*args)
    def use_defaultvalue(*args): return _hk_classes.hk_dsdatavisible_use_defaultvalue(*args)
    def set_defaultvalue(*args): return _hk_classes.hk_dsdatavisible_set_defaultvalue(*args)
    def raw_defaultvalue(*args): return _hk_classes.hk_dsdatavisible_raw_defaultvalue(*args)
    def defaultvalue(*args): return _hk_classes.hk_dsdatavisible_defaultvalue(*args)
    def reset_default(*args): return _hk_classes.hk_dsdatavisible_reset_default(*args)
    def set_numberformat(*args): return _hk_classes.hk_dsdatavisible_set_numberformat(*args)
    def use_numberseparator(*args): return _hk_classes.hk_dsdatavisible_use_numberseparator(*args)
    def commadigits(*args): return _hk_classes.hk_dsdatavisible_commadigits(*args)
    def precision(*args): return _hk_classes.hk_dsdatavisible_precision(*args)
    __swig_getmethods__["set_defaultnumberformat"] = lambda x: _hk_classes.hk_dsdatavisible_set_defaultnumberformat
    if _newclass:set_defaultnumberformat = staticmethod(_hk_classes.hk_dsdatavisible_set_defaultnumberformat)
    __swig_getmethods__["defaultuse_numberseparator"] = lambda x: _hk_classes.hk_dsdatavisible_defaultuse_numberseparator
    if _newclass:defaultuse_numberseparator = staticmethod(_hk_classes.hk_dsdatavisible_defaultuse_numberseparator)
    __swig_getmethods__["defaultprecision"] = lambda x: _hk_classes.hk_dsdatavisible_defaultprecision
    if _newclass:defaultprecision = staticmethod(_hk_classes.hk_dsdatavisible_defaultprecision)
    def value_at(*args): return _hk_classes.hk_dsdatavisible_value_at(*args)
    def value(*args): return _hk_classes.hk_dsdatavisible_value(*args)
    def set_value(*args): return _hk_classes.hk_dsdatavisible_set_value(*args)
    def find(*args): return _hk_classes.hk_dsdatavisible_find(*args)
    def set_on_valuechanged_action(*args): return _hk_classes.hk_dsdatavisible_set_on_valuechanged_action(*args)
    def on_valuechanged_action(*args): return _hk_classes.hk_dsdatavisible_on_valuechanged_action(*args)
    def action_on_valuechanged(*args): return _hk_classes.hk_dsdatavisible_action_on_valuechanged(*args)
    def tag_value(*args): return _hk_classes.hk_dsdatavisible_tag_value(*args)
    def set_filtervalue(*args): return _hk_classes.hk_dsdatavisible_set_filtervalue(*args)
    def filtervalue(*args): return _hk_classes.hk_dsdatavisible_filtervalue(*args)
    __swig_getmethods__["parsed_value"] = lambda x: _hk_classes.hk_dsdatavisible_parsed_value
    if _newclass:parsed_value = staticmethod(_hk_classes.hk_dsdatavisible_parsed_value)
    def update_filter(*args): return _hk_classes.hk_dsdatavisible_update_filter(*args)

class hk_dsdatavisiblePtr(hk_dsdatavisible):
    def __init__(self, this):
        _swig_setattr(self, hk_dsdatavisible, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsdatavisible, 'thisown', 0)
        _swig_setattr(self, hk_dsdatavisible,self.__class__,hk_dsdatavisible)
_hk_classes.hk_dsdatavisible_swigregister(hk_dsdatavisiblePtr)

hk_dsdatavisible_set_defaultnumberformat = _hk_classes.hk_dsdatavisible_set_defaultnumberformat

hk_dsdatavisible_defaultuse_numberseparator = _hk_classes.hk_dsdatavisible_defaultuse_numberseparator

hk_dsdatavisible_defaultprecision = _hk_classes.hk_dsdatavisible_defaultprecision

hk_dsdatavisible_parsed_value = _hk_classes.hk_dsdatavisible_parsed_value

class hk_dslineedit(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dslineedit, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dslineedit, name)
    def __repr__(self):
        return "<C hk_dslineedit instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dslineedit, 'this', _hk_classes.new_hk_dslineedit(*args))
        _swig_setattr(self, hk_dslineedit, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dslineedit):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_gridcolumn(*args): return _hk_classes.hk_dslineedit_set_gridcolumn(*args)
    def gridcolumn(*args): return _hk_classes.hk_dslineedit_gridcolumn(*args)

class hk_dslineeditPtr(hk_dslineedit):
    def __init__(self, this):
        _swig_setattr(self, hk_dslineedit, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dslineedit, 'thisown', 0)
        _swig_setattr(self, hk_dslineedit,self.__class__,hk_dslineedit)
_hk_classes.hk_dslineedit_swigregister(hk_dslineeditPtr)

class hk_dsmemo(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsmemo, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsmemo, name)
    def __repr__(self):
        return "<C hk_dsmemo instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsmemo, 'this', _hk_classes.new_hk_dsmemo(*args))
        _swig_setattr(self, hk_dsmemo, 'thisown', 1)
    def set_gridcolumn(*args): return _hk_classes.hk_dsmemo_set_gridcolumn(*args)
    def gridcolumn(*args): return _hk_classes.hk_dsmemo_gridcolumn(*args)
    def __del__(self, destroy=_hk_classes.delete_hk_dsmemo):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_dsmemoPtr(hk_dsmemo):
    def __init__(self, this):
        _swig_setattr(self, hk_dsmemo, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsmemo, 'thisown', 0)
        _swig_setattr(self, hk_dsmemo,self.__class__,hk_dsmemo)
_hk_classes.hk_dsmemo_swigregister(hk_dsmemoPtr)

class hk_dsboolean(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsboolean, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsboolean, name)
    def __repr__(self):
        return "<C hk_dsboolean instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsboolean, 'this', _hk_classes.new_hk_dsboolean(*args))
        _swig_setattr(self, hk_dsboolean, 'thisown', 1)
    def set_gridcolumn(*args): return _hk_classes.hk_dsboolean_set_gridcolumn(*args)
    def gridcolumn(*args): return _hk_classes.hk_dsboolean_gridcolumn(*args)
    def __del__(self, destroy=_hk_classes.delete_hk_dsboolean):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_dsbooleanPtr(hk_dsboolean):
    def __init__(self, this):
        _swig_setattr(self, hk_dsboolean, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsboolean, 'thisown', 0)
        _swig_setattr(self, hk_dsboolean,self.__class__,hk_dsboolean)
_hk_classes.hk_dsboolean_swigregister(hk_dsbooleanPtr)

class hk_dsdate(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsdate, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsdate, name)
    def __repr__(self):
        return "<C hk_dsdate instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsdate, 'this', _hk_classes.new_hk_dsdate(*args))
        _swig_setattr(self, hk_dsdate, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsdate):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_dsdatePtr(hk_dsdate):
    def __init__(self, this):
        _swig_setattr(self, hk_dsdate, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsdate, 'thisown', 0)
        _swig_setattr(self, hk_dsdate,self.__class__,hk_dsdate)
_hk_classes.hk_dsdate_swigregister(hk_dsdatePtr)

class hk_dsimage(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsimage, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsimage, name)
    def __repr__(self):
        return "<C hk_dsimage instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsimage, 'this', _hk_classes.new_hk_dsimage(*args))
        _swig_setattr(self, hk_dsimage, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsimage):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_path(*args): return _hk_classes.hk_dsimage_set_path(*args)
    def path(*args): return _hk_classes.hk_dsimage_path(*args)
    def savedata(*args): return _hk_classes.hk_dsimage_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dsimage_loaddata(*args)
    def set_value(*args): return _hk_classes.hk_dsimage_set_value(*args)
    def value(*args): return _hk_classes.hk_dsimage_value(*args)
    def value_at(*args): return _hk_classes.hk_dsimage_value_at(*args)
    def set_zoom(*args): return _hk_classes.hk_dsimage_set_zoom(*args)
    def zoom(*args): return _hk_classes.hk_dsimage_zoom(*args)
    def load_localimage(*args): return _hk_classes.hk_dsimage_load_localimage(*args)
    def localimage(*args): return _hk_classes.hk_dsimage_localimage(*args)
    def show_image(*args): return _hk_classes.hk_dsimage_show_image(*args)
    __swig_getmethods__["get_postscript_image"] = lambda x: _hk_classes.hk_dsimage_get_postscript_image
    if _newclass:get_postscript_image = staticmethod(_hk_classes.hk_dsimage_get_postscript_image)
    __swig_getmethods__["get_postscript_image"] = lambda x: _hk_classes.hk_dsimage_get_postscript_image
    if _newclass:get_postscript_image = staticmethod(_hk_classes.hk_dsimage_get_postscript_image)
    __swig_getmethods__["add_postscriptimagetype"] = lambda x: _hk_classes.hk_dsimage_add_postscriptimagetype
    if _newclass:add_postscriptimagetype = staticmethod(_hk_classes.hk_dsimage_add_postscriptimagetype)

class hk_dsimagePtr(hk_dsimage):
    def __init__(self, this):
        _swig_setattr(self, hk_dsimage, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsimage, 'thisown', 0)
        _swig_setattr(self, hk_dsimage,self.__class__,hk_dsimage)
_hk_classes.hk_dsimage_swigregister(hk_dsimagePtr)

hk_dsimage_get_postscript_image = _hk_classes.hk_dsimage_get_postscript_image

hk_dsimage_add_postscriptimagetype = _hk_classes.hk_dsimage_add_postscriptimagetype


currentlineedit = _hk_classes.currentlineedit

currentboolean = _hk_classes.currentboolean

currentcombobox = _hk_classes.currentcombobox

currentmemo = _hk_classes.currentmemo

currentgridcolumn = _hk_classes.currentgridcolumn

currentdsimage = _hk_classes.currentdsimage

currentdsdate = _hk_classes.currentdsdate
class hk_font(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_font, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_font, name)
    def __repr__(self):
        return "<C hk_font instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_font, 'this', _hk_classes.new_hk_font(*args))
        _swig_setattr(self, hk_font, 'thisown', 1)
    def set_font(*args): return _hk_classes.hk_font_set_font(*args)
    def fontname(*args): return _hk_classes.hk_font_fontname(*args)
    def fontsize(*args): return _hk_classes.hk_font_fontsize(*args)
    def set_bold(*args): return _hk_classes.hk_font_set_bold(*args)
    def bold(*args): return _hk_classes.hk_font_bold(*args)
    def set_italic(*args): return _hk_classes.hk_font_set_italic(*args)
    def italic(*args): return _hk_classes.hk_font_italic(*args)
    def psfontname(*args): return _hk_classes.hk_font_psfontname(*args)
    def fontfile(*args): return _hk_classes.hk_font_fontfile(*args)
    def char_ascender_size(*args): return _hk_classes.hk_font_char_ascender_size(*args)
    def char_descender_size(*args): return _hk_classes.hk_font_char_descender_size(*args)
    __swig_getmethods__["set_defaultfont"] = lambda x: _hk_classes.hk_font_set_defaultfont
    if _newclass:set_defaultfont = staticmethod(_hk_classes.hk_font_set_defaultfont)
    __swig_getmethods__["defaultfontname"] = lambda x: _hk_classes.hk_font_defaultfontname
    if _newclass:defaultfontname = staticmethod(_hk_classes.hk_font_defaultfontname)
    __swig_getmethods__["defaultfontsize"] = lambda x: _hk_classes.hk_font_defaultfontsize
    if _newclass:defaultfontsize = staticmethod(_hk_classes.hk_font_defaultfontsize)
    __swig_getmethods__["fontlist"] = lambda x: _hk_classes.hk_font_fontlist
    if _newclass:fontlist = staticmethod(_hk_classes.hk_font_fontlist)
    def __del__(self, destroy=_hk_classes.delete_hk_font):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_fontPtr(hk_font):
    def __init__(self, this):
        _swig_setattr(self, hk_font, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_font, 'thisown', 0)
        _swig_setattr(self, hk_font,self.__class__,hk_font)
_hk_classes.hk_font_swigregister(hk_fontPtr)

hk_font_set_defaultfont = _hk_classes.hk_font_set_defaultfont

hk_font_defaultfontname = _hk_classes.hk_font_defaultfontname

hk_font_defaultfontsize = _hk_classes.hk_font_defaultfontsize

hk_font_fontlist = _hk_classes.hk_font_fontlist

class hk_colour(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_colour, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_colour, name)
    def __repr__(self):
        return "<C hk_colour instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_colour, 'this', _hk_classes.new_hk_colour(*args))
        _swig_setattr(self, hk_colour, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_colour):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_colour(*args): return _hk_classes.hk_colour_set_colour(*args)
    def red(*args): return _hk_classes.hk_colour_red(*args)
    def green(*args): return _hk_classes.hk_colour_green(*args)
    def blue(*args): return _hk_classes.hk_colour_blue(*args)

class hk_colourPtr(hk_colour):
    def __init__(self, this):
        _swig_setattr(self, hk_colour, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_colour, 'thisown', 0)
        _swig_setattr(self, hk_colour,self.__class__,hk_colour)
_hk_classes.hk_colour_swigregister(hk_colourPtr)

class hk_key(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_key, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_key, name)
    def __repr__(self):
        return "<C hk_key instance at %s>" % (self.this,)
    key_Escape = _hk_classes.hk_key_key_Escape
    key_Tab = _hk_classes.hk_key_key_Tab
    key_Backtab = _hk_classes.hk_key_key_Backtab
    key_BackTab = _hk_classes.hk_key_key_BackTab
    key_Backspace = _hk_classes.hk_key_key_Backspace
    key_BackSpace = _hk_classes.hk_key_key_BackSpace
    key_Return = _hk_classes.hk_key_key_Return
    key_Enter = _hk_classes.hk_key_key_Enter
    key_Insert = _hk_classes.hk_key_key_Insert
    key_Delete = _hk_classes.hk_key_key_Delete
    key_Pause = _hk_classes.hk_key_key_Pause
    key_Print = _hk_classes.hk_key_key_Print
    key_SysReq = _hk_classes.hk_key_key_SysReq
    key_Clear = _hk_classes.hk_key_key_Clear
    key_Home = _hk_classes.hk_key_key_Home
    key_End = _hk_classes.hk_key_key_End
    key_Left = _hk_classes.hk_key_key_Left
    key_Up = _hk_classes.hk_key_key_Up
    key_Right = _hk_classes.hk_key_key_Right
    key_Down = _hk_classes.hk_key_key_Down
    key_Prior = _hk_classes.hk_key_key_Prior
    key_PageUp = _hk_classes.hk_key_key_PageUp
    key_Next = _hk_classes.hk_key_key_Next
    key_PageDown = _hk_classes.hk_key_key_PageDown
    key_Shift = _hk_classes.hk_key_key_Shift
    key_Control = _hk_classes.hk_key_key_Control
    key_Meta = _hk_classes.hk_key_key_Meta
    key_Alt = _hk_classes.hk_key_key_Alt
    key_CapsLock = _hk_classes.hk_key_key_CapsLock
    key_NumLock = _hk_classes.hk_key_key_NumLock
    key_ScrollLock = _hk_classes.hk_key_key_ScrollLock
    key_F1 = _hk_classes.hk_key_key_F1
    key_F2 = _hk_classes.hk_key_key_F2
    key_F3 = _hk_classes.hk_key_key_F3
    key_F4 = _hk_classes.hk_key_key_F4
    key_F5 = _hk_classes.hk_key_key_F5
    key_F6 = _hk_classes.hk_key_key_F6
    key_F7 = _hk_classes.hk_key_key_F7
    key_F8 = _hk_classes.hk_key_key_F8
    key_F9 = _hk_classes.hk_key_key_F9
    key_F10 = _hk_classes.hk_key_key_F10
    key_F11 = _hk_classes.hk_key_key_F11
    key_F12 = _hk_classes.hk_key_key_F12
    key_F13 = _hk_classes.hk_key_key_F13
    key_F14 = _hk_classes.hk_key_key_F14
    key_F15 = _hk_classes.hk_key_key_F15
    key_F16 = _hk_classes.hk_key_key_F16
    key_F17 = _hk_classes.hk_key_key_F17
    key_F18 = _hk_classes.hk_key_key_F18
    key_F19 = _hk_classes.hk_key_key_F19
    key_F20 = _hk_classes.hk_key_key_F20
    key_F21 = _hk_classes.hk_key_key_F21
    key_F22 = _hk_classes.hk_key_key_F22
    key_F23 = _hk_classes.hk_key_key_F23
    key_F24 = _hk_classes.hk_key_key_F24
    key_F25 = _hk_classes.hk_key_key_F25
    key_F26 = _hk_classes.hk_key_key_F26
    key_F27 = _hk_classes.hk_key_key_F27
    key_F28 = _hk_classes.hk_key_key_F28
    key_F29 = _hk_classes.hk_key_key_F29
    key_F30 = _hk_classes.hk_key_key_F30
    key_F31 = _hk_classes.hk_key_key_F31
    key_F32 = _hk_classes.hk_key_key_F32
    key_F33 = _hk_classes.hk_key_key_F33
    key_F34 = _hk_classes.hk_key_key_F34
    key_F35 = _hk_classes.hk_key_key_F35
    key_Super_L = _hk_classes.hk_key_key_Super_L
    key_Super_R = _hk_classes.hk_key_key_Super_R
    key_Menu = _hk_classes.hk_key_key_Menu
    key_Hyper_L = _hk_classes.hk_key_key_Hyper_L
    key_Hyper_R = _hk_classes.hk_key_key_Hyper_R
    key_Help = _hk_classes.hk_key_key_Help
    key_Direction_L = _hk_classes.hk_key_key_Direction_L
    key_Direction_R = _hk_classes.hk_key_key_Direction_R
    key_Multi_key = _hk_classes.hk_key_key_Multi_key
    key_Codeinput = _hk_classes.hk_key_key_Codeinput
    key_SingleCandidate = _hk_classes.hk_key_key_SingleCandidate
    key_MultipleCandidate = _hk_classes.hk_key_key_MultipleCandidate
    key_PreviousCandidate = _hk_classes.hk_key_key_PreviousCandidate
    key_Mode_switch = _hk_classes.hk_key_key_Mode_switch
    key_Kanji = _hk_classes.hk_key_key_Kanji
    key_Muhenkan = _hk_classes.hk_key_key_Muhenkan
    key_Henkan = _hk_classes.hk_key_key_Henkan
    key_Romaji = _hk_classes.hk_key_key_Romaji
    key_Hiragana = _hk_classes.hk_key_key_Hiragana
    key_Katakana = _hk_classes.hk_key_key_Katakana
    key_Hiragana_Katakana = _hk_classes.hk_key_key_Hiragana_Katakana
    key_Zenkaku = _hk_classes.hk_key_key_Zenkaku
    key_Hankaku = _hk_classes.hk_key_key_Hankaku
    key_Zenkaku_Hankaku = _hk_classes.hk_key_key_Zenkaku_Hankaku
    key_Touroku = _hk_classes.hk_key_key_Touroku
    key_Massyo = _hk_classes.hk_key_key_Massyo
    key_Kana_Lock = _hk_classes.hk_key_key_Kana_Lock
    key_Kana_Shift = _hk_classes.hk_key_key_Kana_Shift
    key_Eisu_Shift = _hk_classes.hk_key_key_Eisu_Shift
    key_Eisu_toggle = _hk_classes.hk_key_key_Eisu_toggle
    key_Hangul = _hk_classes.hk_key_key_Hangul
    key_Hangul_Start = _hk_classes.hk_key_key_Hangul_Start
    key_Hangul_End = _hk_classes.hk_key_key_Hangul_End
    key_Hangul_Hanja = _hk_classes.hk_key_key_Hangul_Hanja
    key_Hangul_Jamo = _hk_classes.hk_key_key_Hangul_Jamo
    key_Hangul_Romaja = _hk_classes.hk_key_key_Hangul_Romaja
    key_Hangul_Codeinput = _hk_classes.hk_key_key_Hangul_Codeinput
    key_Hangul_Jeonja = _hk_classes.hk_key_key_Hangul_Jeonja
    key_Hangul_Banja = _hk_classes.hk_key_key_Hangul_Banja
    key_Hangul_PreHanja = _hk_classes.hk_key_key_Hangul_PreHanja
    key_Hangul_PostHanja = _hk_classes.hk_key_key_Hangul_PostHanja
    key_Hangul_SingleCandidate = _hk_classes.hk_key_key_Hangul_SingleCandidate
    key_Hangul_MultipleCandidate = _hk_classes.hk_key_key_Hangul_MultipleCandidate
    key_Hangul_PreviousCandidate = _hk_classes.hk_key_key_Hangul_PreviousCandidate
    key_Hangul_Special = _hk_classes.hk_key_key_Hangul_Special
    key_Space = _hk_classes.hk_key_key_Space
    key_Any = _hk_classes.hk_key_key_Any
    key_Exclam = _hk_classes.hk_key_key_Exclam
    key_QuoteDbl = _hk_classes.hk_key_key_QuoteDbl
    key_NumberSign = _hk_classes.hk_key_key_NumberSign
    key_Dollar = _hk_classes.hk_key_key_Dollar
    key_Percent = _hk_classes.hk_key_key_Percent
    key_Ampersand = _hk_classes.hk_key_key_Ampersand
    key_Apostrophe = _hk_classes.hk_key_key_Apostrophe
    key_ParenLeft = _hk_classes.hk_key_key_ParenLeft
    key_ParenRight = _hk_classes.hk_key_key_ParenRight
    key_Asterisk = _hk_classes.hk_key_key_Asterisk
    key_Plus = _hk_classes.hk_key_key_Plus
    key_Comma = _hk_classes.hk_key_key_Comma
    key_Minus = _hk_classes.hk_key_key_Minus
    key_Period = _hk_classes.hk_key_key_Period
    key_Slash = _hk_classes.hk_key_key_Slash
    key_0 = _hk_classes.hk_key_key_0
    key_1 = _hk_classes.hk_key_key_1
    key_2 = _hk_classes.hk_key_key_2
    key_3 = _hk_classes.hk_key_key_3
    key_4 = _hk_classes.hk_key_key_4
    key_5 = _hk_classes.hk_key_key_5
    key_6 = _hk_classes.hk_key_key_6
    key_7 = _hk_classes.hk_key_key_7
    key_8 = _hk_classes.hk_key_key_8
    key_9 = _hk_classes.hk_key_key_9
    key_Colon = _hk_classes.hk_key_key_Colon
    key_Semicolon = _hk_classes.hk_key_key_Semicolon
    key_Less = _hk_classes.hk_key_key_Less
    key_Equal = _hk_classes.hk_key_key_Equal
    key_Greater = _hk_classes.hk_key_key_Greater
    key_Question = _hk_classes.hk_key_key_Question
    key_At = _hk_classes.hk_key_key_At
    key_A = _hk_classes.hk_key_key_A
    key_B = _hk_classes.hk_key_key_B
    key_C = _hk_classes.hk_key_key_C
    key_D = _hk_classes.hk_key_key_D
    key_E = _hk_classes.hk_key_key_E
    key_F = _hk_classes.hk_key_key_F
    key_G = _hk_classes.hk_key_key_G
    key_H = _hk_classes.hk_key_key_H
    key_I = _hk_classes.hk_key_key_I
    key_J = _hk_classes.hk_key_key_J
    key_K = _hk_classes.hk_key_key_K
    key_L = _hk_classes.hk_key_key_L
    key_M = _hk_classes.hk_key_key_M
    key_N = _hk_classes.hk_key_key_N
    key_O = _hk_classes.hk_key_key_O
    key_P = _hk_classes.hk_key_key_P
    key_Q = _hk_classes.hk_key_key_Q
    key_R = _hk_classes.hk_key_key_R
    key_S = _hk_classes.hk_key_key_S
    key_T = _hk_classes.hk_key_key_T
    key_U = _hk_classes.hk_key_key_U
    key_V = _hk_classes.hk_key_key_V
    key_W = _hk_classes.hk_key_key_W
    key_X = _hk_classes.hk_key_key_X
    key_Y = _hk_classes.hk_key_key_Y
    key_Z = _hk_classes.hk_key_key_Z
    key_BracketLeft = _hk_classes.hk_key_key_BracketLeft
    key_Backslash = _hk_classes.hk_key_key_Backslash
    key_BracketRight = _hk_classes.hk_key_key_BracketRight
    key_AsciiCircum = _hk_classes.hk_key_key_AsciiCircum
    key_Underscore = _hk_classes.hk_key_key_Underscore
    key_QuoteLeft = _hk_classes.hk_key_key_QuoteLeft
    key_BraceLeft = _hk_classes.hk_key_key_BraceLeft
    key_Bar = _hk_classes.hk_key_key_Bar
    key_BraceRight = _hk_classes.hk_key_key_BraceRight
    key_AsciiTilde = _hk_classes.hk_key_key_AsciiTilde
    key_nobreakspace = _hk_classes.hk_key_key_nobreakspace
    key_exclamdown = _hk_classes.hk_key_key_exclamdown
    key_cent = _hk_classes.hk_key_key_cent
    key_sterling = _hk_classes.hk_key_key_sterling
    key_currency = _hk_classes.hk_key_key_currency
    key_yen = _hk_classes.hk_key_key_yen
    key_brokenbar = _hk_classes.hk_key_key_brokenbar
    key_section = _hk_classes.hk_key_key_section
    key_diaeresis = _hk_classes.hk_key_key_diaeresis
    key_copyright = _hk_classes.hk_key_key_copyright
    key_ordfeminine = _hk_classes.hk_key_key_ordfeminine
    key_guillemotleft = _hk_classes.hk_key_key_guillemotleft
    key_notsign = _hk_classes.hk_key_key_notsign
    key_hyphen = _hk_classes.hk_key_key_hyphen
    key_registered = _hk_classes.hk_key_key_registered
    key_macron = _hk_classes.hk_key_key_macron
    key_degree = _hk_classes.hk_key_key_degree
    key_plusminus = _hk_classes.hk_key_key_plusminus
    key_twosuperior = _hk_classes.hk_key_key_twosuperior
    key_threesuperior = _hk_classes.hk_key_key_threesuperior
    key_acute = _hk_classes.hk_key_key_acute
    key_mu = _hk_classes.hk_key_key_mu
    key_paragraph = _hk_classes.hk_key_key_paragraph
    key_periodcentered = _hk_classes.hk_key_key_periodcentered
    key_cedilla = _hk_classes.hk_key_key_cedilla
    key_onesuperior = _hk_classes.hk_key_key_onesuperior
    key_masculine = _hk_classes.hk_key_key_masculine
    key_guillemotright = _hk_classes.hk_key_key_guillemotright
    key_onequarter = _hk_classes.hk_key_key_onequarter
    key_onehalf = _hk_classes.hk_key_key_onehalf
    key_threequarters = _hk_classes.hk_key_key_threequarters
    key_questiondown = _hk_classes.hk_key_key_questiondown
    key_Agrave = _hk_classes.hk_key_key_Agrave
    key_Aacute = _hk_classes.hk_key_key_Aacute
    key_Acircumflex = _hk_classes.hk_key_key_Acircumflex
    key_Atilde = _hk_classes.hk_key_key_Atilde
    key_Adiaeresis = _hk_classes.hk_key_key_Adiaeresis
    key_Aring = _hk_classes.hk_key_key_Aring
    key_AE = _hk_classes.hk_key_key_AE
    key_Ccedilla = _hk_classes.hk_key_key_Ccedilla
    key_Egrave = _hk_classes.hk_key_key_Egrave
    key_Eacute = _hk_classes.hk_key_key_Eacute
    key_Ecircumflex = _hk_classes.hk_key_key_Ecircumflex
    key_Ediaeresis = _hk_classes.hk_key_key_Ediaeresis
    key_Igrave = _hk_classes.hk_key_key_Igrave
    key_Iacute = _hk_classes.hk_key_key_Iacute
    key_Icircumflex = _hk_classes.hk_key_key_Icircumflex
    key_Idiaeresis = _hk_classes.hk_key_key_Idiaeresis
    key_ETH = _hk_classes.hk_key_key_ETH
    key_Ntilde = _hk_classes.hk_key_key_Ntilde
    key_Ograve = _hk_classes.hk_key_key_Ograve
    key_Oacute = _hk_classes.hk_key_key_Oacute
    key_Ocircumflex = _hk_classes.hk_key_key_Ocircumflex
    key_Otilde = _hk_classes.hk_key_key_Otilde
    key_Odiaeresis = _hk_classes.hk_key_key_Odiaeresis
    key_multiply = _hk_classes.hk_key_key_multiply
    key_Ooblique = _hk_classes.hk_key_key_Ooblique
    key_Ugrave = _hk_classes.hk_key_key_Ugrave
    key_Uacute = _hk_classes.hk_key_key_Uacute
    key_Ucircumflex = _hk_classes.hk_key_key_Ucircumflex
    key_Udiaeresis = _hk_classes.hk_key_key_Udiaeresis
    key_Yacute = _hk_classes.hk_key_key_Yacute
    key_THORN = _hk_classes.hk_key_key_THORN
    key_ssharp = _hk_classes.hk_key_key_ssharp
    key_agrave = _hk_classes.hk_key_key_agrave
    key_aacute = _hk_classes.hk_key_key_aacute
    key_acircumflex = _hk_classes.hk_key_key_acircumflex
    key_atilde = _hk_classes.hk_key_key_atilde
    key_adiaeresis = _hk_classes.hk_key_key_adiaeresis
    key_aring = _hk_classes.hk_key_key_aring
    key_ae = _hk_classes.hk_key_key_ae
    key_ccedilla = _hk_classes.hk_key_key_ccedilla
    key_egrave = _hk_classes.hk_key_key_egrave
    key_eacute = _hk_classes.hk_key_key_eacute
    key_ecircumflex = _hk_classes.hk_key_key_ecircumflex
    key_ediaeresis = _hk_classes.hk_key_key_ediaeresis
    key_igrave = _hk_classes.hk_key_key_igrave
    key_iacute = _hk_classes.hk_key_key_iacute
    key_icircumflex = _hk_classes.hk_key_key_icircumflex
    key_idiaeresis = _hk_classes.hk_key_key_idiaeresis
    key_eth = _hk_classes.hk_key_key_eth
    key_ntilde = _hk_classes.hk_key_key_ntilde
    key_ograve = _hk_classes.hk_key_key_ograve
    key_oacute = _hk_classes.hk_key_key_oacute
    key_ocircumflex = _hk_classes.hk_key_key_ocircumflex
    key_otilde = _hk_classes.hk_key_key_otilde
    key_odiaeresis = _hk_classes.hk_key_key_odiaeresis
    key_division = _hk_classes.hk_key_key_division
    key_oslash = _hk_classes.hk_key_key_oslash
    key_ugrave = _hk_classes.hk_key_key_ugrave
    key_uacute = _hk_classes.hk_key_key_uacute
    key_ucircumflex = _hk_classes.hk_key_key_ucircumflex
    key_udiaeresis = _hk_classes.hk_key_key_udiaeresis
    key_yacute = _hk_classes.hk_key_key_yacute
    key_thorn = _hk_classes.hk_key_key_thorn
    key_ydiaeresis = _hk_classes.hk_key_key_ydiaeresis
    key_Back = _hk_classes.hk_key_key_Back
    key_Forward = _hk_classes.hk_key_key_Forward
    key_Stop = _hk_classes.hk_key_key_Stop
    key_Refresh = _hk_classes.hk_key_key_Refresh
    key_VolumeDown = _hk_classes.hk_key_key_VolumeDown
    key_VolumeMute = _hk_classes.hk_key_key_VolumeMute
    key_VolumeUp = _hk_classes.hk_key_key_VolumeUp
    key_BassBoost = _hk_classes.hk_key_key_BassBoost
    key_BassUp = _hk_classes.hk_key_key_BassUp
    key_BassDown = _hk_classes.hk_key_key_BassDown
    key_TrebleUp = _hk_classes.hk_key_key_TrebleUp
    key_TrebleDown = _hk_classes.hk_key_key_TrebleDown
    key_MediaPlay = _hk_classes.hk_key_key_MediaPlay
    key_MediaStop = _hk_classes.hk_key_key_MediaStop
    key_MediaPrev = _hk_classes.hk_key_key_MediaPrev
    key_MediaNext = _hk_classes.hk_key_key_MediaNext
    key_MediaRecord = _hk_classes.hk_key_key_MediaRecord
    key_HomePage = _hk_classes.hk_key_key_HomePage
    key_Favorites = _hk_classes.hk_key_key_Favorites
    key_Search = _hk_classes.hk_key_key_Search
    key_Standby = _hk_classes.hk_key_key_Standby
    key_OpenUrl = _hk_classes.hk_key_key_OpenUrl
    key_LaunchMail = _hk_classes.hk_key_key_LaunchMail
    key_LaunchMedia = _hk_classes.hk_key_key_LaunchMedia
    key_Launch0 = _hk_classes.hk_key_key_Launch0
    key_Launch1 = _hk_classes.hk_key_key_Launch1
    key_Launch2 = _hk_classes.hk_key_key_Launch2
    key_Launch3 = _hk_classes.hk_key_key_Launch3
    key_Launch4 = _hk_classes.hk_key_key_Launch4
    key_Launch5 = _hk_classes.hk_key_key_Launch5
    key_Launch6 = _hk_classes.hk_key_key_Launch6
    key_Launch7 = _hk_classes.hk_key_key_Launch7
    key_Launch8 = _hk_classes.hk_key_key_Launch8
    key_Launch9 = _hk_classes.hk_key_key_Launch9
    key_LaunchA = _hk_classes.hk_key_key_LaunchA
    key_LaunchB = _hk_classes.hk_key_key_LaunchB
    key_LaunchC = _hk_classes.hk_key_key_LaunchC
    key_LaunchD = _hk_classes.hk_key_key_LaunchD
    key_LaunchE = _hk_classes.hk_key_key_LaunchE
    key_LaunchF = _hk_classes.hk_key_key_LaunchF
    key_MediaLast = _hk_classes.hk_key_key_MediaLast
    key_unknown = _hk_classes.hk_key_key_unknown
    def __init__(self, *args):
        _swig_setattr(self, hk_key, 'this', _hk_classes.new_hk_key(*args))
        _swig_setattr(self, hk_key, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_key):
        try:
            if self.thisown: destroy(self)
        except: pass
    state_normal = _hk_classes.hk_key_state_normal
    state_shift = _hk_classes.hk_key_state_shift
    state_ctrl = _hk_classes.hk_key_state_ctrl
    state_alt = _hk_classes.hk_key_state_alt
    def state(*args): return _hk_classes.hk_key_state(*args)
    def key(*args): return _hk_classes.hk_key_key(*args)
    def text(*args): return _hk_classes.hk_key_text(*args)
    def __eq__(*args): return _hk_classes.hk_key___eq__(*args)
    def __ne__(*args): return _hk_classes.hk_key___ne__(*args)
    def accept_key(*args): return _hk_classes.hk_key_accept_key(*args)
    def set_accept_key(*args): return _hk_classes.hk_key_set_accept_key(*args)

class hk_keyPtr(hk_key):
    def __init__(self, this):
        _swig_setattr(self, hk_key, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_key, 'thisown', 0)
        _swig_setattr(self, hk_key,self.__class__,hk_key)
_hk_classes.hk_key_swigregister(hk_keyPtr)
hk_aqua = cvar.hk_aqua
hk_beige = cvar.hk_beige
hk_black = cvar.hk_black
hk_blue = cvar.hk_blue
hk_brown = cvar.hk_brown
hk_fuchsia = cvar.hk_fuchsia
hk_darkgrey = cvar.hk_darkgrey
hk_darkyellow = cvar.hk_darkyellow
hk_green = cvar.hk_green
hk_grey = cvar.hk_grey
hk_lightgrey = cvar.hk_lightgrey
hk_lime = cvar.hk_lime
hk_maroon = cvar.hk_maroon
hk_navy = cvar.hk_navy
hk_olive = cvar.hk_olive
hk_orange = cvar.hk_orange
hk_pink = cvar.hk_pink
hk_purple = cvar.hk_purple
hk_red = cvar.hk_red
hk_silver = cvar.hk_silver
hk_teal = cvar.hk_teal
hk_white = cvar.hk_white
hk_yellow = cvar.hk_yellow

class hk_dsmodevisible(hk_dsvisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsvisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsmodevisible, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsvisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsmodevisible, name)
    def __repr__(self):
        return "<C hk_dsmodevisible instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsmodevisible, 'this', _hk_classes.new_hk_dsmodevisible(*args))
        _swig_setattr(self, hk_dsmodevisible, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsmodevisible):
        try:
            if self.thisown: destroy(self)
        except: pass
    designmode = _hk_classes.hk_dsmodevisible_designmode
    viewmode = _hk_classes.hk_dsmodevisible_viewmode
    filtermode = _hk_classes.hk_dsmodevisible_filtermode
    def set_mode(*args): return _hk_classes.hk_dsmodevisible_set_mode(*args)
    def set_designmode(*args): return _hk_classes.hk_dsmodevisible_set_designmode(*args)
    def set_viewmode(*args): return _hk_classes.hk_dsmodevisible_set_viewmode(*args)
    def mode(*args): return _hk_classes.hk_dsmodevisible_mode(*args)
    def has_changed(*args): return _hk_classes.hk_dsmodevisible_has_changed(*args)
    def set_block_has_changed(*args): return _hk_classes.hk_dsmodevisible_set_block_has_changed(*args)
    def block_has_changed(*args): return _hk_classes.hk_dsmodevisible_block_has_changed(*args)
    def reset_has_changed(*args): return _hk_classes.hk_dsmodevisible_reset_has_changed(*args)
    def is_new(*args): return _hk_classes.hk_dsmodevisible_is_new(*args)

class hk_dsmodevisiblePtr(hk_dsmodevisible):
    def __init__(self, this):
        _swig_setattr(self, hk_dsmodevisible, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsmodevisible, 'thisown', 0)
        _swig_setattr(self, hk_dsmodevisible,self.__class__,hk_dsmodevisible)
_hk_classes.hk_dsmodevisible_swigregister(hk_dsmodevisiblePtr)

class hk_dstable(hk_dsmodevisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsmodevisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dstable, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsmodevisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dstable, name)
    def __repr__(self):
        return "<C hk_dstable instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dstable, 'this', _hk_classes.new_hk_dstable(*args))
        _swig_setattr(self, hk_dstable, 'thisown', 1)
    def grid(*args): return _hk_classes.hk_dstable_grid(*args)
    def __del__(self, destroy=_hk_classes.delete_hk_dstable):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_dstablePtr(hk_dstable):
    def __init__(self, this):
        _swig_setattr(self, hk_dstable, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dstable, 'thisown', 0)
        _swig_setattr(self, hk_dstable,self.__class__,hk_dstable)
_hk_classes.hk_dstable_swigregister(hk_dstablePtr)

class hk_dsquery(hk_dsmodevisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsmodevisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsquery, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsmodevisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsquery, name)
    def __repr__(self):
        return "<C hk_dsquery instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsquery, 'this', _hk_classes.new_hk_dsquery(*args))
        _swig_setattr(self, hk_dsquery, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsquery):
        try:
            if self.thisown: destroy(self)
        except: pass
    def grid(*args): return _hk_classes.hk_dsquery_grid(*args)
    def qbe(*args): return _hk_classes.hk_dsquery_qbe(*args)
    def use_qbe(*args): return _hk_classes.hk_dsquery_use_qbe(*args)
    def set_use_qbe(*args): return _hk_classes.hk_dsquery_set_use_qbe(*args)
    def save_query(*args): return _hk_classes.hk_dsquery_save_query(*args)
    def load_query(*args): return _hk_classes.hk_dsquery_load_query(*args)
    def savedata(*args): return _hk_classes.hk_dsquery_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dsquery_loaddata(*args)

class hk_dsqueryPtr(hk_dsquery):
    def __init__(self, this):
        _swig_setattr(self, hk_dsquery, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsquery, 'thisown', 0)
        _swig_setattr(self, hk_dsquery,self.__class__,hk_dsquery)
_hk_classes.hk_dsquery_swigregister(hk_dsqueryPtr)

class hk_presentation(hk_dsmodevisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsmodevisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_presentation, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsmodevisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_presentation, name)
    def __repr__(self):
        return "<C hk_presentation instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_presentation, 'this', _hk_classes.new_hk_presentation(*args))
        _swig_setattr(self, hk_presentation, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_presentation):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_database(*args): return _hk_classes.hk_presentation_set_database(*args)
    def database(*args): return _hk_classes.hk_presentation_database(*args)
    general = _hk_classes.hk_presentation_general
    form = _hk_classes.hk_presentation_form
    report = _hk_classes.hk_presentation_report
    qbe = _hk_classes.hk_presentation_qbe
    referentialintegrity = _hk_classes.hk_presentation_referentialintegrity
    def presentationtype(*args): return _hk_classes.hk_presentation_presentationtype(*args)
    def set_name(*args): return _hk_classes.hk_presentation_set_name(*args)
    def name(*args): return _hk_classes.hk_presentation_name(*args)
    def set_mode(*args): return _hk_classes.hk_presentation_set_mode(*args)
    def datasources(*args): return _hk_classes.hk_presentation_datasources(*args)
    def new_datasource(*args): return _hk_classes.hk_presentation_new_datasource(*args)
    def get_datasource(*args): return _hk_classes.hk_presentation_get_datasource(*args)
    def get_datasource_by_shortname(*args): return _hk_classes.hk_presentation_get_datasource_by_shortname(*args)
    def unique_datasourcename(*args): return _hk_classes.hk_presentation_unique_datasourcename(*args)
    def unique_shortdatasourcename(*args): return _hk_classes.hk_presentation_unique_shortdatasourcename(*args)
    def savedata(*args): return _hk_classes.hk_presentation_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_presentation_loaddata(*args)
    def clear_datasourcelist(*args): return _hk_classes.hk_presentation_clear_datasourcelist(*args)
    def set_designsize(*args): return _hk_classes.hk_presentation_set_designsize(*args)
    def set_presentationsize(*args): return _hk_classes.hk_presentation_set_presentationsize(*args)
    def designwidth(*args): return _hk_classes.hk_presentation_designwidth(*args)
    def designheight(*args): return _hk_classes.hk_presentation_designheight(*args)
    def horizontal2relativ(*args): return _hk_classes.hk_presentation_horizontal2relativ(*args)
    def vertical2relativ(*args): return _hk_classes.hk_presentation_vertical2relativ(*args)
    def relativ2horizontal(*args): return _hk_classes.hk_presentation_relativ2horizontal(*args)
    def relativ2vertical(*args): return _hk_classes.hk_presentation_relativ2vertical(*args)
    def register_object(*args): return _hk_classes.hk_presentation_register_object(*args)
    def set_automatic_enable_datasources(*args): return _hk_classes.hk_presentation_set_automatic_enable_datasources(*args)
    def automatic_enable_datasources(*args): return _hk_classes.hk_presentation_automatic_enable_datasources(*args)
    bulkfont = _hk_classes.hk_presentation_bulkfont
    bulkforeground = _hk_classes.hk_presentation_bulkforeground
    bulkbackground = _hk_classes.hk_presentation_bulkbackground
    def bulk_operation(*args): return _hk_classes.hk_presentation_bulk_operation(*args)
    relative = _hk_classes.hk_presentation_relative
    absolute = _hk_classes.hk_presentation_absolute
    def set_sizetype(*args): return _hk_classes.hk_presentation_set_sizetype(*args)
    def sizetype(*args): return _hk_classes.hk_presentation_sizetype(*args)
    __swig_getmethods__["set_defaultsizetype"] = lambda x: _hk_classes.hk_presentation_set_defaultsizetype
    if _newclass:set_defaultsizetype = staticmethod(_hk_classes.hk_presentation_set_defaultsizetype)
    __swig_getmethods__["defaultsizetype"] = lambda x: _hk_classes.hk_presentation_defaultsizetype
    if _newclass:defaultsizetype = staticmethod(_hk_classes.hk_presentation_defaultsizetype)
    __swig_getmethods__["set_snap2gridx"] = lambda x: _hk_classes.hk_presentation_set_snap2gridx
    if _newclass:set_snap2gridx = staticmethod(_hk_classes.hk_presentation_set_snap2gridx)
    __swig_getmethods__["snap2gridx"] = lambda x: _hk_classes.hk_presentation_snap2gridx
    if _newclass:snap2gridx = staticmethod(_hk_classes.hk_presentation_snap2gridx)
    __swig_getmethods__["set_snap2gridy"] = lambda x: _hk_classes.hk_presentation_set_snap2gridy
    if _newclass:set_snap2gridy = staticmethod(_hk_classes.hk_presentation_set_snap2gridy)
    __swig_getmethods__["snap2gridy"] = lambda x: _hk_classes.hk_presentation_snap2gridy
    if _newclass:snap2gridy = staticmethod(_hk_classes.hk_presentation_snap2gridy)
    def interpreter(*args): return _hk_classes.hk_presentation_interpreter(*args)
    def script_error(*args): return _hk_classes.hk_presentation_script_error(*args)
    def set_interpretername(*args): return _hk_classes.hk_presentation_set_interpretername(*args)
    def interpretername(*args): return _hk_classes.hk_presentation_interpretername(*args)
    def enable_datasources(*args): return _hk_classes.hk_presentation_enable_datasources(*args)
    def is_subpresentation(*args): return _hk_classes.hk_presentation_is_subpresentation(*args)
    def while_loading(*args): return _hk_classes.hk_presentation_while_loading(*args)
    def intervaltasks(*args): return _hk_classes.hk_presentation_intervaltasks(*args)
    def load_module(*args): return _hk_classes.hk_presentation_load_module(*args)

class hk_presentationPtr(hk_presentation):
    def __init__(self, this):
        _swig_setattr(self, hk_presentation, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_presentation, 'thisown', 0)
        _swig_setattr(self, hk_presentation,self.__class__,hk_presentation)
_hk_classes.hk_presentation_swigregister(hk_presentationPtr)

hk_presentation_set_defaultsizetype = _hk_classes.hk_presentation_set_defaultsizetype

hk_presentation_defaultsizetype = _hk_classes.hk_presentation_defaultsizetype

hk_presentation_set_snap2gridx = _hk_classes.hk_presentation_set_snap2gridx

hk_presentation_snap2gridx = _hk_classes.hk_presentation_snap2gridx

hk_presentation_set_snap2gridy = _hk_classes.hk_presentation_set_snap2gridy

hk_presentation_snap2gridy = _hk_classes.hk_presentation_snap2gridy

class hk_button(hk_dsvisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsvisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_button, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsvisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_button, name)
    def __repr__(self):
        return "<C hk_button instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_button, 'this', _hk_classes.new_hk_button(*args))
        _swig_setattr(self, hk_button, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_button):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_action(*args): return _hk_classes.hk_button_set_action(*args)
    def push_action(*args): return _hk_classes.hk_button_push_action(*args)
    def object(*args): return _hk_classes.hk_button_object(*args)
    def action(*args): return _hk_classes.hk_button_action(*args)
    def show_maximized(*args): return _hk_classes.hk_button_show_maximized(*args)
    def set_database(*args): return _hk_classes.hk_button_set_database(*args)
    def database(*args): return _hk_classes.hk_button_database(*args)
    def show_icon(*args): return _hk_classes.hk_button_show_icon(*args)
    def set_is_togglebutton(*args): return _hk_classes.hk_button_set_is_togglebutton(*args)
    def is_togglebutton(*args): return _hk_classes.hk_button_is_togglebutton(*args)
    def is_pushed(*args): return _hk_classes.hk_button_is_pushed(*args)
    def set_is_pushed(*args): return _hk_classes.hk_button_set_is_pushed(*args)

class hk_buttonPtr(hk_button):
    def __init__(self, this):
        _swig_setattr(self, hk_button, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_button, 'thisown', 0)
        _swig_setattr(self, hk_button,self.__class__,hk_button)
_hk_classes.hk_button_swigregister(hk_buttonPtr)


currentbutton = _hk_classes.currentbutton
class hk_form(hk_presentation):
    __swig_setmethods__ = {}
    for _s in [hk_presentation]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_form, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_presentation]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_form, name)
    def __repr__(self):
        return "<C hk_form instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_form, 'this', _hk_classes.new_hk_form(*args))
        _swig_setattr(self, hk_form, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_form):
        try:
            if self.thisown: destroy(self)
        except: pass
    def new_grid(*args): return _hk_classes.hk_form_new_grid(*args)
    def new_lineedit(*args): return _hk_classes.hk_form_new_lineedit(*args)
    def new_bool(*args): return _hk_classes.hk_form_new_bool(*args)
    def new_combobox(*args): return _hk_classes.hk_form_new_combobox(*args)
    def new_button(*args): return _hk_classes.hk_form_new_button(*args)
    def new_rowselector(*args): return _hk_classes.hk_form_new_rowselector(*args)
    def new_memo(*args): return _hk_classes.hk_form_new_memo(*args)
    def new_label(*args): return _hk_classes.hk_form_new_label(*args)
    def new_subform(*args): return _hk_classes.hk_form_new_subform(*args)
    def new_date(*args): return _hk_classes.hk_form_new_date(*args)
    def new_image(*args): return _hk_classes.hk_form_new_image(*args)
    def new_tabvisible(*args): return _hk_classes.hk_form_new_tabvisible(*args)
    def save_form(*args): return _hk_classes.hk_form_save_form(*args)
    def load_form(*args): return _hk_classes.hk_form_load_form(*args)
    def savedata(*args): return _hk_classes.hk_form_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_form_loaddata(*args)
    def set_designsize(*args): return _hk_classes.hk_form_set_designsize(*args)
    def get_visible(*args): return _hk_classes.hk_form_get_visible(*args)
    def bulk_operation(*args): return _hk_classes.hk_form_bulk_operation(*args)
    def set_mode(*args): return _hk_classes.hk_form_set_mode(*args)
    def masterform(*args): return _hk_classes.hk_form_masterform(*args)
    def clear_visiblelist(*args): return _hk_classes.hk_form_clear_visiblelist(*args)
    def set_taborder(*args): return _hk_classes.hk_form_set_taborder(*args)
    def taborder(*args): return _hk_classes.hk_form_taborder(*args)
    def taborder_previous(*args): return _hk_classes.hk_form_taborder_previous(*args)
    def taborder_next(*args): return _hk_classes.hk_form_taborder_next(*args)
    def first_tabobject(*args): return _hk_classes.hk_form_first_tabobject(*args)
    def last_tabobject(*args): return _hk_classes.hk_form_last_tabobject(*args)
    def goto_taborder_next(*args): return _hk_classes.hk_form_goto_taborder_next(*args)
    def goto_taborder_previous(*args): return _hk_classes.hk_form_goto_taborder_previous(*args)
    def goto_taborder_first(*args): return _hk_classes.hk_form_goto_taborder_first(*args)
    def goto_taborder_last(*args): return _hk_classes.hk_form_goto_taborder_last(*args)
    def set_focus(*args): return _hk_classes.hk_form_set_focus(*args)
    def visibles(*args): return _hk_classes.hk_form_visibles(*args)
    def is_subform(*args): return _hk_classes.hk_form_is_subform(*args)
    def is_subpresentation(*args): return _hk_classes.hk_form_is_subpresentation(*args)
    def show_asdialog(*args): return _hk_classes.hk_form_show_asdialog(*args)
    def screen_width(*args): return _hk_classes.hk_form_screen_width(*args)
    def screen_height(*args): return _hk_classes.hk_form_screen_height(*args)
    def while_load_form(*args): return _hk_classes.hk_form_while_load_form(*args)
    def while_loading(*args): return _hk_classes.hk_form_while_loading(*args)
    include_data = _hk_classes.hk_form_include_data
    no_data = _hk_classes.hk_form_no_data
    def form2html(*args): return _hk_classes.hk_form_form2html(*args)
    def get_pyvisible(*args): return _hk_classes.hk_form_get_pyvisible(*args)

class hk_formPtr(hk_form):
    def __init__(self, this):
        _swig_setattr(self, hk_form, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_form, 'thisown', 0)
        _swig_setattr(self, hk_form,self.__class__,hk_form)
_hk_classes.hk_form_swigregister(hk_formPtr)


currentform = _hk_classes.currentform
class hk_label(hk_visible):
    __swig_setmethods__ = {}
    for _s in [hk_visible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_label, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_visible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_label, name)
    def __repr__(self):
        return "<C hk_label instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_label, 'this', _hk_classes.new_hk_label(*args))
        _swig_setattr(self, hk_label, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_label):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_topline(*args): return _hk_classes.hk_label_set_topline(*args)
    def set_bottomline(*args): return _hk_classes.hk_label_set_bottomline(*args)
    def set_leftline(*args): return _hk_classes.hk_label_set_leftline(*args)
    def set_rightline(*args): return _hk_classes.hk_label_set_rightline(*args)
    def set_diagonalluro(*args): return _hk_classes.hk_label_set_diagonalluro(*args)
    def set_diagonalloru(*args): return _hk_classes.hk_label_set_diagonalloru(*args)
    def diagonalloru(*args): return _hk_classes.hk_label_diagonalloru(*args)
    def topline(*args): return _hk_classes.hk_label_topline(*args)
    def bottomline(*args): return _hk_classes.hk_label_bottomline(*args)
    def leftline(*args): return _hk_classes.hk_label_leftline(*args)
    def rightline(*args): return _hk_classes.hk_label_rightline(*args)
    def diagonalluro(*args): return _hk_classes.hk_label_diagonalluro(*args)
    def set_frame(*args): return _hk_classes.hk_label_set_frame(*args)

class hk_labelPtr(hk_label):
    def __init__(self, this):
        _swig_setattr(self, hk_label, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_label, 'thisown', 0)
        _swig_setattr(self, hk_label,self.__class__,hk_label)
_hk_classes.hk_label_swigregister(hk_labelPtr)

class hk_report(hk_presentation):
    __swig_setmethods__ = {}
    for _s in [hk_presentation]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_report, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_presentation]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_report, name)
    def __repr__(self):
        return "<C hk_report instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_report, 'this', _hk_classes.new_hk_report(*args))
        _swig_setattr(self, hk_report, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_report):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_filename(*args): return _hk_classes.hk_report_set_filename(*args)
    def filename(*args): return _hk_classes.hk_report_filename(*args)
    def set_use_standard_storagepath(*args): return _hk_classes.hk_report_set_use_standard_storagepath(*args)
    def use_standard_storagepath(*args): return _hk_classes.hk_report_use_standard_storagepath(*args)
    def outputfile(*args): return _hk_classes.hk_report_outputfile(*args)
    def set_begin(*args): return _hk_classes.hk_report_set_begin(*args)
    def begin(*args): return _hk_classes.hk_report_begin(*args)
    def set_end(*args): return _hk_classes.hk_report_set_end(*args)
    def end(*args): return _hk_classes.hk_report_end(*args)
    def page_header(*args): return _hk_classes.hk_report_page_header(*args)
    def page_footer(*args): return _hk_classes.hk_report_page_footer(*args)
    def report_header(*args): return _hk_classes.hk_report_report_header(*args)
    def report_footer(*args): return _hk_classes.hk_report_report_footer(*args)
    def datasection(*args): return _hk_classes.hk_report_datasection(*args)
    def new_sectionpair(*args): return _hk_classes.hk_report_new_sectionpair(*args)
    def new_sectionpair_at(*args): return _hk_classes.hk_report_new_sectionpair_at(*args)
    def move_sectionpair(*args): return _hk_classes.hk_report_move_sectionpair(*args)
    def execute(*args): return _hk_classes.hk_report_execute(*args)
    def while_executing(*args): return _hk_classes.hk_report_while_executing(*args)
    def while_load_report(*args): return _hk_classes.hk_report_while_load_report(*args)
    def while_loading(*args): return _hk_classes.hk_report_while_loading(*args)
    def set_mode(*args): return _hk_classes.hk_report_set_mode(*args)
    def set_recodefunction(*args): return _hk_classes.hk_report_set_recodefunction(*args)
    def recodefunction(*args): return _hk_classes.hk_report_recodefunction(*args)
    def recodefunctionstring(*args): return _hk_classes.hk_report_recodefunctionstring(*args)
    __swig_getmethods__["add_recodetype"] = lambda x: _hk_classes.hk_report_add_recodetype
    if _newclass:add_recodetype = staticmethod(_hk_classes.hk_report_add_recodetype)
    __swig_getmethods__["recodelist"] = lambda x: _hk_classes.hk_report_recodelist
    if _newclass:recodelist = staticmethod(_hk_classes.hk_report_recodelist)
    def new_page(*args): return _hk_classes.hk_report_new_page(*args)
    def set_pagedelimiter(*args): return _hk_classes.hk_report_set_pagedelimiter(*args)
    def pagedelimiter(*args): return _hk_classes.hk_report_pagedelimiter(*args)
    def sectionpairs(*args): return _hk_classes.hk_report_sectionpairs(*args)
    def set_multiplefiles(*args): return _hk_classes.hk_report_set_multiplefiles(*args)
    def multiplefiles(*args): return _hk_classes.hk_report_multiplefiles(*args)
    def set_fileextension(*args): return _hk_classes.hk_report_set_fileextension(*args)
    def fileextension(*args): return _hk_classes.hk_report_fileextension(*args)
    def set_firstpagenumber(*args): return _hk_classes.hk_report_set_firstpagenumber(*args)
    def firstpagenumber(*args): return _hk_classes.hk_report_firstpagenumber(*args)
    def set_pagenamecolumn(*args): return _hk_classes.hk_report_set_pagenamecolumn(*args)
    def pagenamecolumn(*args): return _hk_classes.hk_report_pagenamecolumn(*args)
    def set_filebegin(*args): return _hk_classes.hk_report_set_filebegin(*args)
    def filebegin(*args): return _hk_classes.hk_report_filebegin(*args)
    def set_fileend(*args): return _hk_classes.hk_report_set_fileend(*args)
    def fileend(*args): return _hk_classes.hk_report_fileend(*args)
    def set_periodic(*args): return _hk_classes.hk_report_set_periodic(*args)
    def periodic_when(*args): return _hk_classes.hk_report_periodic_when(*args)
    def periodic_what(*args): return _hk_classes.hk_report_periodic_what(*args)
    def periodic_newpage(*args): return _hk_classes.hk_report_periodic_newpage(*args)
    def outputstream(*args): return _hk_classes.hk_report_outputstream(*args)
    def set_masterreport(*args): return _hk_classes.hk_report_set_masterreport(*args)
    def masterreport(*args): return _hk_classes.hk_report_masterreport(*args)
    def is_subreport(*args): return _hk_classes.hk_report_is_subreport(*args)
    def is_subpresentation(*args): return _hk_classes.hk_report_is_subpresentation(*args)
    def rowcount(*args): return _hk_classes.hk_report_rowcount(*args)
    def current_rowcount(*args): return _hk_classes.hk_report_current_rowcount(*args)
    def font(*args): return _hk_classes.hk_report_font(*args)
    def pagenumber(*args): return _hk_classes.hk_report_pagenumber(*args)
    def absolutepagenumber(*args): return _hk_classes.hk_report_absolutepagenumber(*args)
    def rownumber(*args): return _hk_classes.hk_report_rownumber(*args)
    def set_presentationdatasource(*args): return _hk_classes.hk_report_set_presentationdatasource(*args)
    def save_report(*args): return _hk_classes.hk_report_save_report(*args)
    def load_report(*args): return _hk_classes.hk_report_load_report(*args)
    def savedata(*args): return _hk_classes.hk_report_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_report_loaddata(*args)
    def default_reportdata(*args): return _hk_classes.hk_report_default_reportdata(*args)
    def set_default_reportdata(*args): return _hk_classes.hk_report_set_default_reportdata(*args)
    def default_beforereportdata(*args): return _hk_classes.hk_report_default_beforereportdata(*args)
    def set_default_beforereportdata(*args): return _hk_classes.hk_report_set_default_beforereportdata(*args)
    def default_afterreportdata(*args): return _hk_classes.hk_report_default_afterreportdata(*args)
    def set_default_afterreportdata(*args): return _hk_classes.hk_report_set_default_afterreportdata(*args)
    def default_reportprecision(*args): return _hk_classes.hk_report_default_reportprecision(*args)
    def set_default_reportprecision(*args): return _hk_classes.hk_report_set_default_reportprecision(*args)
    def default_use_reportseparator(*args): return _hk_classes.hk_report_default_use_reportseparator(*args)
    def set_default_use_reportseparator(*args): return _hk_classes.hk_report_set_default_use_reportseparator(*args)
    def set_default_reportdataconfigurefunction(*args): return _hk_classes.hk_report_set_default_reportdataconfigurefunction(*args)
    def default_reportdataconfigurefunction(*args): return _hk_classes.hk_report_default_reportdataconfigurefunction(*args)
    def set_default_reportsectionbegin(*args): return _hk_classes.hk_report_set_default_reportsectionbegin(*args)
    def default_reportsectionbegin(*args): return _hk_classes.hk_report_default_reportsectionbegin(*args)
    def set_default_reportsectionend(*args): return _hk_classes.hk_report_set_default_reportsectionend(*args)
    def default_reportsectionend(*args): return _hk_classes.hk_report_default_reportsectionend(*args)
    def set_default_reportsectionbetweendata(*args): return _hk_classes.hk_report_set_default_reportsectionbetweendata(*args)
    def default_reportsectionbetweendata(*args): return _hk_classes.hk_report_default_reportsectionbetweendata(*args)
    def set_default_reportsectioncountfunction(*args): return _hk_classes.hk_report_set_default_reportsectioncountfunction(*args)
    def default_reportsectioncountfunction(*args): return _hk_classes.hk_report_default_reportsectioncountfunction(*args)
    def set_topline(*args): return _hk_classes.hk_report_set_topline(*args)
    def topline(*args): return _hk_classes.hk_report_topline(*args)
    def set_bottomline(*args): return _hk_classes.hk_report_set_bottomline(*args)
    def bottomline(*args): return _hk_classes.hk_report_bottomline(*args)
    def set_leftline(*args): return _hk_classes.hk_report_set_leftline(*args)
    def leftline(*args): return _hk_classes.hk_report_leftline(*args)
    def set_rightline(*args): return _hk_classes.hk_report_set_rightline(*args)
    def rightline(*args): return _hk_classes.hk_report_rightline(*args)
    def set_diagonalluro(*args): return _hk_classes.hk_report_set_diagonalluro(*args)
    def diagonalluro(*args): return _hk_classes.hk_report_diagonalluro(*args)
    def set_diagonalloru(*args): return _hk_classes.hk_report_set_diagonalloru(*args)
    def diagonalloru(*args): return _hk_classes.hk_report_diagonalloru(*args)
    def set_frame(*args): return _hk_classes.hk_report_set_frame(*args)
    def set_borders(*args): return _hk_classes.hk_report_set_borders(*args)
    def border_left(*args): return _hk_classes.hk_report_border_left(*args)
    def border_right(*args): return _hk_classes.hk_report_border_right(*args)
    def border_top(*args): return _hk_classes.hk_report_border_top(*args)
    def border_bottom(*args): return _hk_classes.hk_report_border_bottom(*args)
    landscape = _hk_classes.hk_report_landscape
    portrait = _hk_classes.hk_report_portrait
    def set_orientation(*args): return _hk_classes.hk_report_set_orientation(*args)
    def orientation(*args): return _hk_classes.hk_report_orientation(*args)
    A0 = _hk_classes.hk_report_A0
    A1 = _hk_classes.hk_report_A1
    A2 = _hk_classes.hk_report_A2
    A3 = _hk_classes.hk_report_A3
    A4 = _hk_classes.hk_report_A4
    A5 = _hk_classes.hk_report_A5
    A6 = _hk_classes.hk_report_A6
    A7 = _hk_classes.hk_report_A7
    A8 = _hk_classes.hk_report_A8
    A9 = _hk_classes.hk_report_A9
    A10 = _hk_classes.hk_report_A10
    letter = _hk_classes.hk_report_letter
    legal = _hk_classes.hk_report_legal
    executive = _hk_classes.hk_report_executive
    tabloid = _hk_classes.hk_report_tabloid
    userdefined = _hk_classes.hk_report_userdefined
    def set_pageformat(*args): return _hk_classes.hk_report_set_pageformat(*args)
    def pageformat(*args): return _hk_classes.hk_report_pageformat(*args)
    def formatwidth(*args): return _hk_classes.hk_report_formatwidth(*args)
    def formatheight(*args): return _hk_classes.hk_report_formatheight(*args)
    def set_reportconfigurefunction(*args): return _hk_classes.hk_report_set_reportconfigurefunction(*args)
    def set_reporttype(*args): return _hk_classes.hk_report_set_reporttype(*args)
    def reporttype(*args): return _hk_classes.hk_report_reporttype(*args)
    __swig_getmethods__["add_reporttype"] = lambda x: _hk_classes.hk_report_add_reporttype
    if _newclass:add_reporttype = staticmethod(_hk_classes.hk_report_add_reporttype)
    __swig_getmethods__["reporttypelist"] = lambda x: _hk_classes.hk_report_reporttypelist
    if _newclass:reporttypelist = staticmethod(_hk_classes.hk_report_reporttypelist)
    def configure_page(*args): return _hk_classes.hk_report_configure_page(*args)
    def neutralize_definition(*args): return _hk_classes.hk_report_neutralize_definition(*args)
    def init_report(*args): return _hk_classes.hk_report_init_report(*args)
    __swig_getmethods__["add_configurefunctiontype"] = lambda x: _hk_classes.hk_report_add_configurefunctiontype
    if _newclass:add_configurefunctiontype = staticmethod(_hk_classes.hk_report_add_configurefunctiontype)
    __swig_getmethods__["configurefunctionlist"] = lambda x: _hk_classes.hk_report_configurefunctionlist
    if _newclass:configurefunctionlist = staticmethod(_hk_classes.hk_report_configurefunctionlist)
    def remove_sectionpair(*args): return _hk_classes.hk_report_remove_sectionpair(*args)
    def neededfonts(*args): return _hk_classes.hk_report_neededfonts(*args)
    def usedfonts(*args): return _hk_classes.hk_report_usedfonts(*args)
    def usedpsfonts(*args): return _hk_classes.hk_report_usedpsfonts(*args)
    __swig_getmethods__["set_printcommand"] = lambda x: _hk_classes.hk_report_set_printcommand
    if _newclass:set_printcommand = staticmethod(_hk_classes.hk_report_set_printcommand)
    __swig_getmethods__["printcommand"] = lambda x: _hk_classes.hk_report_printcommand
    if _newclass:printcommand = staticmethod(_hk_classes.hk_report_printcommand)
    def print_report(*args): return _hk_classes.hk_report_print_report(*args)
    def designwidth(*args): return _hk_classes.hk_report_designwidth(*args)
    def designheight(*args): return _hk_classes.hk_report_designheight(*args)
    def horizontal2relativ(*args): return _hk_classes.hk_report_horizontal2relativ(*args)
    def vertical2relativ(*args): return _hk_classes.hk_report_vertical2relativ(*args)
    def relativ2horizontal(*args): return _hk_classes.hk_report_relativ2horizontal(*args)
    def relativ2vertical(*args): return _hk_classes.hk_report_relativ2vertical(*args)
    def bulk_operation(*args): return _hk_classes.hk_report_bulk_operation(*args)
    __swig_getmethods__["setup_reportbasics"] = lambda x: _hk_classes.hk_report_setup_reportbasics
    if _newclass:setup_reportbasics = staticmethod(_hk_classes.hk_report_setup_reportbasics)
    def get_reportdatavisible(*args): return _hk_classes.hk_report_get_reportdatavisible(*args)
    def set_on_new_page_action(*args): return _hk_classes.hk_report_set_on_new_page_action(*args)
    def on_new_page_action(*args): return _hk_classes.hk_report_on_new_page_action(*args)
    def stop_execution(*args): return _hk_classes.hk_report_stop_execution(*args)
    def execution_stopped(*args): return _hk_classes.hk_report_execution_stopped(*args)
    def filelocale(*args): return _hk_classes.hk_report_filelocale(*args)
    def filecharset(*args): return _hk_classes.hk_report_filecharset(*args)
    def set_filelocale(*args): return _hk_classes.hk_report_set_filelocale(*args)
    def set_filecharset(*args): return _hk_classes.hk_report_set_filecharset(*args)
    def set_useencodingtab(*args): return _hk_classes.hk_report_set_useencodingtab(*args)
    def useencodingtab(*args): return _hk_classes.hk_report_useencodingtab(*args)
    def encodingtab(*args): return _hk_classes.hk_report_encodingtab(*args)
    def set_print_full_pages_only(*args): return _hk_classes.hk_report_set_print_full_pages_only(*args)
    def print_full_pages_only(*args): return _hk_classes.hk_report_print_full_pages_only(*args)
    def set_fullpagereplacefunction(*args): return _hk_classes.hk_report_set_fullpagereplacefunction(*args)
    def fullpagereplacefunction(*args): return _hk_classes.hk_report_fullpagereplacefunction(*args)
    def fullpagereplacefunctionstring(*args): return _hk_classes.hk_report_fullpagereplacefunctionstring(*args)
    __swig_getmethods__["add_fullpagereplacefunctiontype"] = lambda x: _hk_classes.hk_report_add_fullpagereplacefunctiontype
    if _newclass:add_fullpagereplacefunctiontype = staticmethod(_hk_classes.hk_report_add_fullpagereplacefunctiontype)
    __swig_getmethods__["fullpagereplacefunctionlist"] = lambda x: _hk_classes.hk_report_fullpagereplacefunctionlist
    if _newclass:fullpagereplacefunctionlist = staticmethod(_hk_classes.hk_report_fullpagereplacefunctionlist)
    __swig_getmethods__["set_embedfonts"] = lambda x: _hk_classes.hk_report_set_embedfonts
    if _newclass:set_embedfonts = staticmethod(_hk_classes.hk_report_set_embedfonts)
    __swig_getmethods__["embedfonts"] = lambda x: _hk_classes.hk_report_embedfonts
    if _newclass:embedfonts = staticmethod(_hk_classes.hk_report_embedfonts)
    def tag_value(*args): return _hk_classes.hk_report_tag_value(*args)

class hk_reportPtr(hk_report):
    def __init__(self, this):
        _swig_setattr(self, hk_report, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_report, 'thisown', 0)
        _swig_setattr(self, hk_report,self.__class__,hk_report)
_hk_classes.hk_report_swigregister(hk_reportPtr)

hk_report_add_recodetype = _hk_classes.hk_report_add_recodetype

hk_report_recodelist = _hk_classes.hk_report_recodelist

hk_report_add_reporttype = _hk_classes.hk_report_add_reporttype

hk_report_reporttypelist = _hk_classes.hk_report_reporttypelist

hk_report_add_configurefunctiontype = _hk_classes.hk_report_add_configurefunctiontype

hk_report_configurefunctionlist = _hk_classes.hk_report_configurefunctionlist

hk_report_set_printcommand = _hk_classes.hk_report_set_printcommand

hk_report_printcommand = _hk_classes.hk_report_printcommand

hk_report_setup_reportbasics = _hk_classes.hk_report_setup_reportbasics

hk_report_add_fullpagereplacefunctiontype = _hk_classes.hk_report_add_fullpagereplacefunctiontype

hk_report_fullpagereplacefunctionlist = _hk_classes.hk_report_fullpagereplacefunctionlist

hk_report_set_embedfonts = _hk_classes.hk_report_set_embedfonts

hk_report_embedfonts = _hk_classes.hk_report_embedfonts


currentreport = _hk_classes.currentreport
class struct_countnumbers(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, struct_countnumbers, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, struct_countnumbers, name)
    def __repr__(self):
        return "<C struct_countnumbers instance at %s>" % (self.this,)
    __swig_setmethods__["sum"] = _hk_classes.struct_countnumbers_sum_set
    __swig_getmethods__["sum"] = _hk_classes.struct_countnumbers_sum_get
    if _newclass:sum = property(_hk_classes.struct_countnumbers_sum_get, _hk_classes.struct_countnumbers_sum_set)
    __swig_setmethods__["min"] = _hk_classes.struct_countnumbers_min_set
    __swig_getmethods__["min"] = _hk_classes.struct_countnumbers_min_get
    if _newclass:min = property(_hk_classes.struct_countnumbers_min_get, _hk_classes.struct_countnumbers_min_set)
    __swig_setmethods__["max"] = _hk_classes.struct_countnumbers_max_set
    __swig_getmethods__["max"] = _hk_classes.struct_countnumbers_max_get
    if _newclass:max = property(_hk_classes.struct_countnumbers_max_get, _hk_classes.struct_countnumbers_max_set)
    __swig_setmethods__["value"] = _hk_classes.struct_countnumbers_value_set
    __swig_getmethods__["value"] = _hk_classes.struct_countnumbers_value_get
    if _newclass:value = property(_hk_classes.struct_countnumbers_value_get, _hk_classes.struct_countnumbers_value_set)
    __swig_setmethods__["count"] = _hk_classes.struct_countnumbers_count_set
    __swig_getmethods__["count"] = _hk_classes.struct_countnumbers_count_get
    if _newclass:count = property(_hk_classes.struct_countnumbers_count_get, _hk_classes.struct_countnumbers_count_set)
    __swig_setmethods__["squaresum"] = _hk_classes.struct_countnumbers_squaresum_set
    __swig_getmethods__["squaresum"] = _hk_classes.struct_countnumbers_squaresum_get
    if _newclass:squaresum = property(_hk_classes.struct_countnumbers_squaresum_get, _hk_classes.struct_countnumbers_squaresum_set)
    __swig_setmethods__["colname"] = _hk_classes.struct_countnumbers_colname_set
    __swig_getmethods__["colname"] = _hk_classes.struct_countnumbers_colname_get
    if _newclass:colname = property(_hk_classes.struct_countnumbers_colname_get, _hk_classes.struct_countnumbers_colname_set)
    def __init__(self, *args):
        _swig_setattr(self, struct_countnumbers, 'this', _hk_classes.new_struct_countnumbers(*args))
        _swig_setattr(self, struct_countnumbers, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_struct_countnumbers):
        try:
            if self.thisown: destroy(self)
        except: pass

class struct_countnumbersPtr(struct_countnumbers):
    def __init__(self, this):
        _swig_setattr(self, struct_countnumbers, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, struct_countnumbers, 'thisown', 0)
        _swig_setattr(self, struct_countnumbers,self.__class__,struct_countnumbers)
_hk_classes.struct_countnumbers_swigregister(struct_countnumbersPtr)

class hk_reportsection(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_reportsection, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_reportsection, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_reportsection instance at %s>" % (self.this,)
    def set_unique(*args): return _hk_classes.hk_reportsection_set_unique(*args)
    def unique(*args): return _hk_classes.hk_reportsection_unique(*args)
    def new_data(*args): return _hk_classes.hk_reportsection_new_data(*args)
    def data_at(*args): return _hk_classes.hk_reportsection_data_at(*args)
    def datalist(*args): return _hk_classes.hk_reportsection_datalist(*args)
    def actual_string(*args): return _hk_classes.hk_reportsection_actual_string(*args)
    def sectionbegin(*args): return _hk_classes.hk_reportsection_sectionbegin(*args)
    def set_sectionbegin(*args): return _hk_classes.hk_reportsection_set_sectionbegin(*args)
    def sectionend(*args): return _hk_classes.hk_reportsection_sectionend(*args)
    def set_sectionend(*args): return _hk_classes.hk_reportsection_set_sectionend(*args)
    def betweendata(*args): return _hk_classes.hk_reportsection_betweendata(*args)
    def set_betweendata(*args): return _hk_classes.hk_reportsection_set_betweendata(*args)
    def new_uniquevalue(*args): return _hk_classes.hk_reportsection_new_uniquevalue(*args)
    def endsection(*args): return _hk_classes.hk_reportsection_endsection(*args)
    def set_automatic_create_data(*args): return _hk_classes.hk_reportsection_set_automatic_create_data(*args)
    def new_page_after_section(*args): return _hk_classes.hk_reportsection_new_page_after_section(*args)
    def set_new_page_after_section(*args): return _hk_classes.hk_reportsection_set_new_page_after_section(*args)
    def section_was_printed(*args): return _hk_classes.hk_reportsection_section_was_printed(*args)
    def set_subreport(*args): return _hk_classes.hk_reportsection_set_subreport(*args)
    def subreportname(*args): return _hk_classes.hk_reportsection_subreportname(*args)
    def subreport(*args): return _hk_classes.hk_reportsection_subreport(*args)
    def add_depending_fields(*args): return _hk_classes.hk_reportsection_add_depending_fields(*args)
    def clear_depending_fields(*args): return _hk_classes.hk_reportsection_clear_depending_fields(*args)
    def depending_on_thisreportfields(*args): return _hk_classes.hk_reportsection_depending_on_thisreportfields(*args)
    def depending_on_subreportfields(*args): return _hk_classes.hk_reportsection_depending_on_subreportfields(*args)
    def print_subreport_before_data(*args): return _hk_classes.hk_reportsection_print_subreport_before_data(*args)
    def print_subreport(*args): return _hk_classes.hk_reportsection_print_subreport(*args)
    def set_replacefunction(*args): return _hk_classes.hk_reportsection_set_replacefunction(*args)
    def replace(*args): return _hk_classes.hk_reportsection_replace(*args)
    __swig_getmethods__["add_sectionreplacefunctiontype"] = lambda x: _hk_classes.hk_reportsection_add_sectionreplacefunctiontype
    if _newclass:add_sectionreplacefunctiontype = staticmethod(_hk_classes.hk_reportsection_add_sectionreplacefunctiontype)
    __swig_getmethods__["sectionreplacefunctionlist"] = lambda x: _hk_classes.hk_reportsection_sectionreplacefunctionlist
    if _newclass:sectionreplacefunctionlist = staticmethod(_hk_classes.hk_reportsection_sectionreplacefunctionlist)
    def reset_count(*args): return _hk_classes.hk_reportsection_reset_count(*args)
    def font(*args): return _hk_classes.hk_reportsection_font(*args)
    def counts_as(*args): return _hk_classes.hk_reportsection_counts_as(*args)
    __swig_getmethods__["add_sectioncountfunctiontype"] = lambda x: _hk_classes.hk_reportsection_add_sectioncountfunctiontype
    if _newclass:add_sectioncountfunctiontype = staticmethod(_hk_classes.hk_reportsection_add_sectioncountfunctiontype)
    __swig_getmethods__["sectioncountfunctionlist"] = lambda x: _hk_classes.hk_reportsection_sectioncountfunctionlist
    if _newclass:sectioncountfunctionlist = staticmethod(_hk_classes.hk_reportsection_sectioncountfunctionlist)
    def set_sectioncountfunction(*args): return _hk_classes.hk_reportsection_set_sectioncountfunction(*args)
    def sectioncountfunctionstring(*args): return _hk_classes.hk_reportsection_sectioncountfunctionstring(*args)
    def sectioncountfunction(*args): return _hk_classes.hk_reportsection_sectioncountfunction(*args)
    def savedata(*args): return _hk_classes.hk_reportsection_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_reportsection_loaddata(*args)
    def set_datasource(*args): return _hk_classes.hk_reportsection_set_datasource(*args)
    def set_presentationdatasource(*args): return _hk_classes.hk_reportsection_set_presentationdatasource(*args)
    def default_reportdata(*args): return _hk_classes.hk_reportsection_default_reportdata(*args)
    def set_default_reportdata(*args): return _hk_classes.hk_reportsection_set_default_reportdata(*args)
    def default_beforereportdata(*args): return _hk_classes.hk_reportsection_default_beforereportdata(*args)
    def set_default_beforereportdata(*args): return _hk_classes.hk_reportsection_set_default_beforereportdata(*args)
    def default_afterreportdata(*args): return _hk_classes.hk_reportsection_default_afterreportdata(*args)
    def set_default_afterreportdata(*args): return _hk_classes.hk_reportsection_set_default_afterreportdata(*args)
    def set_default_reportdataconfigurefunction(*args): return _hk_classes.hk_reportsection_set_default_reportdataconfigurefunction(*args)
    def default_reportdataconfigurefunction(*args): return _hk_classes.hk_reportsection_default_reportdataconfigurefunction(*args)
    def default_reportprecision(*args): return _hk_classes.hk_reportsection_default_reportprecision(*args)
    def set_default_reportprecision(*args): return _hk_classes.hk_reportsection_set_default_reportprecision(*args)
    def default_use_reportseparator(*args): return _hk_classes.hk_reportsection_default_use_reportseparator(*args)
    def set_default_use_reportseparator(*args): return _hk_classes.hk_reportsection_set_default_use_reportseparator(*args)
    def neutralize_definition(*args): return _hk_classes.hk_reportsection_neutralize_definition(*args)
    def reset(*args): return _hk_classes.hk_reportsection_reset(*args)
    def clear_countingfields(*args): return _hk_classes.hk_reportsection_clear_countingfields(*args)
    def remove_data(*args): return _hk_classes.hk_reportsection_remove_data(*args)
    def report(*args): return _hk_classes.hk_reportsection_report(*args)
    def horizontal2relativ(*args): return _hk_classes.hk_reportsection_horizontal2relativ(*args)
    def vertical2relativ(*args): return _hk_classes.hk_reportsection_vertical2relativ(*args)
    def relativ2horizontal(*args): return _hk_classes.hk_reportsection_relativ2horizontal(*args)
    def relativ2vertical(*args): return _hk_classes.hk_reportsection_relativ2vertical(*args)
    def reportsectionpair(*args): return _hk_classes.hk_reportsection_reportsectionpair(*args)
    def set_offset(*args): return _hk_classes.hk_reportsection_set_offset(*args)
    def offset(*args): return _hk_classes.hk_reportsection_offset(*args)
    def bulk_operation(*args): return _hk_classes.hk_reportsection_bulk_operation(*args)
    def set_automaticcreatedatafunction(*args): return _hk_classes.hk_reportsection_set_automaticcreatedatafunction(*args)
    def automaticcreatedatafunction(*args): return _hk_classes.hk_reportsection_automaticcreatedatafunction(*args)
    def get_reportdatavisible(*args): return _hk_classes.hk_reportsection_get_reportdatavisible(*args)
    def is_fully_printed(*args): return _hk_classes.hk_reportsection_is_fully_printed(*args)
    def set_print_section(*args): return _hk_classes.hk_reportsection_set_print_section(*args)
    def print_section(*args): return _hk_classes.hk_reportsection_print_section(*args)

class hk_reportsectionPtr(hk_reportsection):
    def __init__(self, this):
        _swig_setattr(self, hk_reportsection, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_reportsection, 'thisown', 0)
        _swig_setattr(self, hk_reportsection,self.__class__,hk_reportsection)
_hk_classes.hk_reportsection_swigregister(hk_reportsectionPtr)

hk_reportsection_add_sectionreplacefunctiontype = _hk_classes.hk_reportsection_add_sectionreplacefunctiontype

hk_reportsection_sectionreplacefunctionlist = _hk_classes.hk_reportsection_sectionreplacefunctionlist

hk_reportsection_add_sectioncountfunctiontype = _hk_classes.hk_reportsection_add_sectioncountfunctiontype

hk_reportsection_sectioncountfunctionlist = _hk_classes.hk_reportsection_sectioncountfunctionlist

class hk_reportsectionpair(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_reportsectionpair, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_reportsectionpair, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_reportsectionpair instance at %s>" % (self.this,)
    def __del__(self, destroy=_hk_classes.delete_hk_reportsectionpair):
        try:
            if self.thisown: destroy(self)
        except: pass
    def headersection(*args): return _hk_classes.hk_reportsectionpair_headersection(*args)
    def footersection(*args): return _hk_classes.hk_reportsectionpair_footersection(*args)
    def set_sections(*args): return _hk_classes.hk_reportsectionpair_set_sections(*args)
    def init_sections(*args): return _hk_classes.hk_reportsectionpair_init_sections(*args)
    def set_columnname(*args): return _hk_classes.hk_reportsectionpair_set_columnname(*args)
    def set_ascending_order(*args): return _hk_classes.hk_reportsectionpair_set_ascending_order(*args)
    def ascending_order(*args): return _hk_classes.hk_reportsectionpair_ascending_order(*args)
    def columnname(*args): return _hk_classes.hk_reportsectionpair_columnname(*args)
    def reset(*args): return _hk_classes.hk_reportsectionpair_reset(*args)
    def clear_countingfields(*args): return _hk_classes.hk_reportsectionpair_clear_countingfields(*args)
    def savedata(*args): return _hk_classes.hk_reportsectionpair_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_reportsectionpair_loaddata(*args)
    def set_presentationdatasource(*args): return _hk_classes.hk_reportsectionpair_set_presentationdatasource(*args)
    def presentationdatasource(*args): return _hk_classes.hk_reportsectionpair_presentationdatasource(*args)
    def neutralize_definition(*args): return _hk_classes.hk_reportsectionpair_neutralize_definition(*args)
    def bulk_operation(*args): return _hk_classes.hk_reportsectionpair_bulk_operation(*args)

class hk_reportsectionpairPtr(hk_reportsectionpair):
    def __init__(self, this):
        _swig_setattr(self, hk_reportsectionpair, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_reportsectionpair, 'thisown', 0)
        _swig_setattr(self, hk_reportsectionpair,self.__class__,hk_reportsectionpair)
_hk_classes.hk_reportsectionpair_swigregister(hk_reportsectionpairPtr)

class hk_reportdata(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_reportdata, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_reportdata, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_reportdata instance at %s>" % (self.this,)
    def set_data(*args): return _hk_classes.hk_reportdata_set_data(*args)
    def data(*args): return _hk_classes.hk_reportdata_data(*args)
    def set_beforedata(*args): return _hk_classes.hk_reportdata_set_beforedata(*args)
    def set_afterdata(*args): return _hk_classes.hk_reportdata_set_afterdata(*args)
    def beforedata(*args): return _hk_classes.hk_reportdata_beforedata(*args)
    def afterdata(*args): return _hk_classes.hk_reportdata_afterdata(*args)
    def actual_string(*args): return _hk_classes.hk_reportdata_actual_string(*args)
    def count(*args): return _hk_classes.hk_reportdata_count(*args)
    def set_runningcount(*args): return _hk_classes.hk_reportdata_set_runningcount(*args)
    def runningcount(*args): return _hk_classes.hk_reportdata_runningcount(*args)
    def font(*args): return _hk_classes.hk_reportdata_font(*args)
    def set_datacountfunction(*args): return _hk_classes.hk_reportdata_set_datacountfunction(*args)
    def datacountfunctionstring(*args): return _hk_classes.hk_reportdata_datacountfunctionstring(*args)
    def datacountfunction(*args): return _hk_classes.hk_reportdata_datacountfunction(*args)
    def set_topline(*args): return _hk_classes.hk_reportdata_set_topline(*args)
    def set_bottomline(*args): return _hk_classes.hk_reportdata_set_bottomline(*args)
    def set_leftline(*args): return _hk_classes.hk_reportdata_set_leftline(*args)
    def set_rightline(*args): return _hk_classes.hk_reportdata_set_rightline(*args)
    def set_diagonalluro(*args): return _hk_classes.hk_reportdata_set_diagonalluro(*args)
    def set_diagonalloru(*args): return _hk_classes.hk_reportdata_set_diagonalloru(*args)
    def diagonalloru(*args): return _hk_classes.hk_reportdata_diagonalloru(*args)
    def topline(*args): return _hk_classes.hk_reportdata_topline(*args)
    def bottomline(*args): return _hk_classes.hk_reportdata_bottomline(*args)
    def leftline(*args): return _hk_classes.hk_reportdata_leftline(*args)
    def rightline(*args): return _hk_classes.hk_reportdata_rightline(*args)
    def diagonalluro(*args): return _hk_classes.hk_reportdata_diagonalluro(*args)
    def set_frame(*args): return _hk_classes.hk_reportdata_set_frame(*args)
    def set_linebreak(*args): return _hk_classes.hk_reportdata_set_linebreak(*args)
    def linebreak(*args): return _hk_classes.hk_reportdata_linebreak(*args)
    def set_dynamic_height(*args): return _hk_classes.hk_reportdata_set_dynamic_height(*args)
    def dynamic_height(*args): return _hk_classes.hk_reportdata_dynamic_height(*args)
    def set_configurefunction(*args): return _hk_classes.hk_reportdata_set_configurefunction(*args)
    def configurefunction(*args): return _hk_classes.hk_reportdata_configurefunction(*args)
    def configurefunctionstring(*args): return _hk_classes.hk_reportdata_configurefunctionstring(*args)
    def savedata(*args): return _hk_classes.hk_reportdata_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_reportdata_loaddata(*args)
    def neutralize_definition(*args): return _hk_classes.hk_reportdata_neutralize_definition(*args)
    __swig_getmethods__["add_configurefunctiontype"] = lambda x: _hk_classes.hk_reportdata_add_configurefunctiontype
    if _newclass:add_configurefunctiontype = staticmethod(_hk_classes.hk_reportdata_add_configurefunctiontype)
    __swig_getmethods__["configurefunctionlist"] = lambda x: _hk_classes.hk_reportdata_configurefunctionlist
    if _newclass:configurefunctionlist = staticmethod(_hk_classes.hk_reportdata_configurefunctionlist)
    __swig_getmethods__["add_datacountfunctiontype"] = lambda x: _hk_classes.hk_reportdata_add_datacountfunctiontype
    if _newclass:add_datacountfunctiontype = staticmethod(_hk_classes.hk_reportdata_add_datacountfunctiontype)
    __swig_getmethods__["datacountfunctionlist"] = lambda x: _hk_classes.hk_reportdata_datacountfunctionlist
    if _newclass:datacountfunctionlist = staticmethod(_hk_classes.hk_reportdata_datacountfunctionlist)
    def set_replacefunction(*args): return _hk_classes.hk_reportdata_set_replacefunction(*args)
    __swig_getmethods__["add_datareplacefunctiontype"] = lambda x: _hk_classes.hk_reportdata_add_datareplacefunctiontype
    if _newclass:add_datareplacefunctiontype = staticmethod(_hk_classes.hk_reportdata_add_datareplacefunctiontype)
    __swig_getmethods__["datareplacefunctionlist"] = lambda x: _hk_classes.hk_reportdata_datareplacefunctionlist
    if _newclass:datareplacefunctionlist = staticmethod(_hk_classes.hk_reportdata_datareplacefunctionlist)
    def reset_count(*args): return _hk_classes.hk_reportdata_reset_count(*args)
    def report(*args): return _hk_classes.hk_reportdata_report(*args)
    def section(*args): return _hk_classes.hk_reportdata_section(*args)
    def lower_widget(*args): return _hk_classes.hk_reportdata_lower_widget(*args)
    def raise_widget(*args): return _hk_classes.hk_reportdata_raise_widget(*args)
    def set_on_print_action(*args): return _hk_classes.hk_reportdata_set_on_print_action(*args)
    def on_print_action(*args): return _hk_classes.hk_reportdata_on_print_action(*args)
    def value_in_lines(*args): return _hk_classes.hk_reportdata_value_in_lines(*args)
    def height(*args): return _hk_classes.hk_reportdata_height(*args)
    def data_fully_printed(*args): return _hk_classes.hk_reportdata_data_fully_printed(*args)
    def reset_data_fully_printed(*args): return _hk_classes.hk_reportdata_reset_data_fully_printed(*args)
    def pagescount(*args): return _hk_classes.hk_reportdata_pagescount(*args)
    def tag_value(*args): return _hk_classes.hk_reportdata_tag_value(*args)
    def column_value(*args): return _hk_classes.hk_reportdata_column_value(*args)
    def set_is_image(*args): return _hk_classes.hk_reportdata_set_is_image(*args)
    def is_image(*args): return _hk_classes.hk_reportdata_is_image(*args)

class hk_reportdataPtr(hk_reportdata):
    def __init__(self, this):
        _swig_setattr(self, hk_reportdata, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_reportdata, 'thisown', 0)
        _swig_setattr(self, hk_reportdata,self.__class__,hk_reportdata)
_hk_classes.hk_reportdata_swigregister(hk_reportdataPtr)

hk_reportdata_add_configurefunctiontype = _hk_classes.hk_reportdata_add_configurefunctiontype

hk_reportdata_configurefunctionlist = _hk_classes.hk_reportdata_configurefunctionlist

hk_reportdata_add_datacountfunctiontype = _hk_classes.hk_reportdata_add_datacountfunctiontype

hk_reportdata_datacountfunctionlist = _hk_classes.hk_reportdata_datacountfunctionlist

hk_reportdata_add_datareplacefunctiontype = _hk_classes.hk_reportdata_add_datareplacefunctiontype

hk_reportdata_datareplacefunctionlist = _hk_classes.hk_reportdata_datareplacefunctionlist


currentreportdata = _hk_classes.currentreportdata
class hk_actionquery(hk_data):
    __swig_setmethods__ = {}
    for _s in [hk_data]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_actionquery, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_data]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_actionquery, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C hk_actionquery instance at %s>" % (self.this,)
    def __del__(self, destroy=_hk_classes.delete_hk_actionquery):
        try:
            if self.thisown: destroy(self)
        except: pass
    def type(*args): return _hk_classes.hk_actionquery_type(*args)
    def set_sql(*args): return _hk_classes.hk_actionquery_set_sql(*args)
    def sql(*args): return _hk_classes.hk_actionquery_sql(*args)
    def length(*args): return _hk_classes.hk_actionquery_length(*args)
    def execute(*args): return _hk_classes.hk_actionquery_execute(*args)

class hk_actionqueryPtr(hk_actionquery):
    def __init__(self, this):
        _swig_setattr(self, hk_actionquery, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_actionquery, 'thisown', 0)
        _swig_setattr(self, hk_actionquery,self.__class__,hk_actionquery)
_hk_classes.hk_actionquery_swigregister(hk_actionqueryPtr)

class hk_dsgrid(hk_dsvisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsvisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsgrid, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsvisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsgrid, name)
    def __repr__(self):
        return "<C hk_dsgrid instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dsgrid, 'this', _hk_classes.new_hk_dsgrid(*args))
        _swig_setattr(self, hk_dsgrid, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsgrid):
        try:
            if self.thisown: destroy(self)
        except: pass
    def gridcolumn(*args): return _hk_classes.hk_dsgrid_gridcolumn(*args)
    def columnscount(*args): return _hk_classes.hk_dsgrid_columnscount(*args)
    def set_gridcolumnwidth(*args): return _hk_classes.hk_dsgrid_set_gridcolumnwidth(*args)
    def change_columnposition(*args): return _hk_classes.hk_dsgrid_change_columnposition(*args)
    def set_gridcolumns(*args): return _hk_classes.hk_dsgrid_set_gridcolumns(*args)
    def clear_gridcolumn(*args): return _hk_classes.hk_dsgrid_clear_gridcolumn(*args)
    def set_hold_rowdefinition(*args): return _hk_classes.hk_dsgrid_set_hold_rowdefinition(*args)
    def set_enablingbehaviour(*args): return _hk_classes.hk_dsgrid_set_enablingbehaviour(*args)
    def savedata(*args): return _hk_classes.hk_dsgrid_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dsgrid_loaddata(*args)
    def set_datasource(*args): return _hk_classes.hk_dsgrid_set_datasource(*args)
    def visible_columns(*args): return _hk_classes.hk_dsgrid_visible_columns(*args)
    def nonvisible_columns(*args): return _hk_classes.hk_dsgrid_nonvisible_columns(*args)
    def save_table(*args): return _hk_classes.hk_dsgrid_save_table(*args)
    def load_table(*args): return _hk_classes.hk_dsgrid_load_table(*args)
    def save_view(*args): return _hk_classes.hk_dsgrid_save_view(*args)
    def load_view(*args): return _hk_classes.hk_dsgrid_load_view(*args)
    def set_rowheight(*args): return _hk_classes.hk_dsgrid_set_rowheight(*args)
    def rowheight(*args): return _hk_classes.hk_dsgrid_rowheight(*args)

class hk_dsgridPtr(hk_dsgrid):
    def __init__(self, this):
        _swig_setattr(self, hk_dsgrid, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsgrid, 'thisown', 0)
        _swig_setattr(self, hk_dsgrid,self.__class__,hk_dsgrid)
_hk_classes.hk_dsgrid_swigregister(hk_dsgridPtr)

class hk_dsgridcolumn(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dsgridcolumn, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dsgridcolumn, name)
    def __repr__(self):
        return "<C hk_dsgridcolumn instance at %s>" % (self.this,)
    columnedit = _hk_classes.hk_dsgridcolumn_columnedit
    columnbool = _hk_classes.hk_dsgridcolumn_columnbool
    columncombo = _hk_classes.hk_dsgridcolumn_columncombo
    def __init__(self, *args):
        _swig_setattr(self, hk_dsgridcolumn, 'this', _hk_classes.new_hk_dsgridcolumn(*args))
        _swig_setattr(self, hk_dsgridcolumn, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dsgridcolumn):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_displayname(*args): return _hk_classes.hk_dsgridcolumn_set_displayname(*args)
    def displayname(*args): return _hk_classes.hk_dsgridcolumn_displayname(*args)
    def set_columntype(*args): return _hk_classes.hk_dsgridcolumn_set_columntype(*args)
    def columntype(*args): return _hk_classes.hk_dsgridcolumn_columntype(*args)
    def set_columnwidth(*args): return _hk_classes.hk_dsgridcolumn_set_columnwidth(*args)
    def columnwidth(*args): return _hk_classes.hk_dsgridcolumn_columnwidth(*args)
    def savedata(*args): return _hk_classes.hk_dsgridcolumn_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dsgridcolumn_loaddata(*args)
    def set_listdatasource(*args): return _hk_classes.hk_dsgridcolumn_set_listdatasource(*args)
    def listdatasource(*args): return _hk_classes.hk_dsgridcolumn_listdatasource(*args)
    def listdatasourcename(*args): return _hk_classes.hk_dsgridcolumn_listdatasourcename(*args)
    def listdatasourcetype(*args): return _hk_classes.hk_dsgridcolumn_listdatasourcetype(*args)
    def set_viewcolumnname(*args): return _hk_classes.hk_dsgridcolumn_set_viewcolumnname(*args)
    def viewcolumnname(*args): return _hk_classes.hk_dsgridcolumn_viewcolumnname(*args)
    def set_listcolumnname(*args): return _hk_classes.hk_dsgridcolumn_set_listcolumnname(*args)
    def listcolumnname(*args): return _hk_classes.hk_dsgridcolumn_listcolumnname(*args)
    def set_listpresentationdatasource(*args): return _hk_classes.hk_dsgridcolumn_set_listpresentationdatasource(*args)
    def listpresentationdatasource(*args): return _hk_classes.hk_dsgridcolumn_listpresentationdatasource(*args)
    def value_at(*args): return _hk_classes.hk_dsgridcolumn_value_at(*args)
    def set_datasource(*args): return _hk_classes.hk_dsgridcolumn_set_datasource(*args)
    def combobox(*args): return _hk_classes.hk_dsgridcolumn_combobox(*args)
    def find(*args): return _hk_classes.hk_dsgridcolumn_find(*args)
    def is_findstring(*args): return _hk_classes.hk_dsgridcolumn_is_findstring(*args)
    def action_on_click(*args): return _hk_classes.hk_dsgridcolumn_action_on_click(*args)
    def action_on_doubleclick(*args): return _hk_classes.hk_dsgridcolumn_action_on_doubleclick(*args)
    def action_on_close(*args): return _hk_classes.hk_dsgridcolumn_action_on_close(*args)
    def action_on_open(*args): return _hk_classes.hk_dsgridcolumn_action_on_open(*args)
    def action_before_store_changed_data(*args): return _hk_classes.hk_dsgridcolumn_action_before_store_changed_data(*args)
    def action_after_store_changed_data(*args): return _hk_classes.hk_dsgridcolumn_action_after_store_changed_data(*args)
    def action_before_row_change(*args): return _hk_classes.hk_dsgridcolumn_action_before_row_change(*args)
    def action_after_row_change(*args): return _hk_classes.hk_dsgridcolumn_action_after_row_change(*args)
    def action_before_delete(*args): return _hk_classes.hk_dsgridcolumn_action_before_delete(*args)
    def action_after_delete(*args): return _hk_classes.hk_dsgridcolumn_action_after_delete(*args)
    def action_before_insert(*args): return _hk_classes.hk_dsgridcolumn_action_before_insert(*args)
    def action_after_insert(*args): return _hk_classes.hk_dsgridcolumn_action_after_insert(*args)
    def action_on_getfocus(*args): return _hk_classes.hk_dsgridcolumn_action_on_getfocus(*args)
    def action_on_loosefocus(*args): return _hk_classes.hk_dsgridcolumn_action_on_loosefocus(*args)
    def action_on_key(*args): return _hk_classes.hk_dsgridcolumn_action_on_key(*args)
    def action_on_select(*args): return _hk_classes.hk_dsgridcolumn_action_on_select(*args)
    def on_select_action(*args): return _hk_classes.hk_dsgridcolumn_on_select_action(*args)
    def set_on_select_action(*args): return _hk_classes.hk_dsgridcolumn_set_on_select_action(*args)

class hk_dsgridcolumnPtr(hk_dsgridcolumn):
    def __init__(self, this):
        _swig_setattr(self, hk_dsgridcolumn, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dsgridcolumn, 'thisown', 0)
        _swig_setattr(self, hk_dsgridcolumn,self.__class__,hk_dsgridcolumn)
_hk_classes.hk_dsgridcolumn_swigregister(hk_dsgridcolumnPtr)
default_cellwidth = cvar.default_cellwidth
default_cellheight = cvar.default_cellheight

class hk_dscombobox(hk_dsdatavisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_dscombobox, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsdatavisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_dscombobox, name)
    def __repr__(self):
        return "<C hk_dscombobox instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_dscombobox, 'this', _hk_classes.new_hk_dscombobox(*args))
        _swig_setattr(self, hk_dscombobox, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_dscombobox):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_listdatasource(*args): return _hk_classes.hk_dscombobox_set_listdatasource(*args)
    def listdatasource(*args): return _hk_classes.hk_dscombobox_listdatasource(*args)
    def set_datasource(*args): return _hk_classes.hk_dscombobox_set_datasource(*args)
    def set_viewcolumnname(*args): return _hk_classes.hk_dscombobox_set_viewcolumnname(*args)
    def viewcolumnname(*args): return _hk_classes.hk_dscombobox_viewcolumnname(*args)
    def set_listcolumnname(*args): return _hk_classes.hk_dscombobox_set_listcolumnname(*args)
    def listcolumnname(*args): return _hk_classes.hk_dscombobox_listcolumnname(*args)
    def savedata(*args): return _hk_classes.hk_dscombobox_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_dscombobox_loaddata(*args)
    def set_listpresentationdatasource(*args): return _hk_classes.hk_dscombobox_set_listpresentationdatasource(*args)
    def listpresentationdatasource(*args): return _hk_classes.hk_dscombobox_listpresentationdatasource(*args)
    def load_listitems(*args): return _hk_classes.hk_dscombobox_load_listitems(*args)
    combo = _hk_classes.hk_dscombobox_combo
    combo_noedit = _hk_classes.hk_dscombobox_combo_noedit
    selector = _hk_classes.hk_dscombobox_selector
    def set_mode(*args): return _hk_classes.hk_dscombobox_set_mode(*args)
    def mode(*args): return _hk_classes.hk_dscombobox_mode(*args)
    def value_at(*args): return _hk_classes.hk_dscombobox_value_at(*args)
    def viewcolumn(*args): return _hk_classes.hk_dscombobox_viewcolumn(*args)
    def listcolumn(*args): return _hk_classes.hk_dscombobox_listcolumn(*args)
    def set_gridcolumn(*args): return _hk_classes.hk_dscombobox_set_gridcolumn(*args)
    def gridcolumn(*args): return _hk_classes.hk_dscombobox_gridcolumn(*args)
    def set_use_textlist(*args): return _hk_classes.hk_dscombobox_set_use_textlist(*args)
    def use_textlist(*args): return _hk_classes.hk_dscombobox_use_textlist(*args)
    def set_textlist(*args): return _hk_classes.hk_dscombobox_set_textlist(*args)
    def add_text(*args): return _hk_classes.hk_dscombobox_add_text(*args)
    def textlist(*args): return _hk_classes.hk_dscombobox_textlist(*args)
    def action_on_select(*args): return _hk_classes.hk_dscombobox_action_on_select(*args)
    def on_select_action(*args): return _hk_classes.hk_dscombobox_on_select_action(*args)
    def set_on_select_action(*args): return _hk_classes.hk_dscombobox_set_on_select_action(*args)

class hk_dscomboboxPtr(hk_dscombobox):
    def __init__(self, this):
        _swig_setattr(self, hk_dscombobox, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_dscombobox, 'thisown', 0)
        _swig_setattr(self, hk_dscombobox,self.__class__,hk_dscombobox)
_hk_classes.hk_dscombobox_swigregister(hk_dscomboboxPtr)

class hk_subform(hk_dsvisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsvisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_subform, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsvisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_subform, name)
    def __repr__(self):
        return "<C hk_subform instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_subform, 'this', _hk_classes.new_hk_subform(*args))
        _swig_setattr(self, hk_subform, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_subform):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_name(*args): return _hk_classes.hk_subform_set_name(*args)
    def name(*args): return _hk_classes.hk_subform_name(*args)
    def subform(*args): return _hk_classes.hk_subform_subform(*args)
    def add_depending_fields(*args): return _hk_classes.hk_subform_add_depending_fields(*args)
    def clear_depending_fields(*args): return _hk_classes.hk_subform_clear_depending_fields(*args)
    def activate_subform(*args): return _hk_classes.hk_subform_activate_subform(*args)
    def depending_on_fields(*args): return _hk_classes.hk_subform_depending_on_fields(*args)
    def savedata(*args): return _hk_classes.hk_subform_savedata(*args)
    def loaddata(*args): return _hk_classes.hk_subform_loaddata(*args)

class hk_subformPtr(hk_subform):
    def __init__(self, this):
        _swig_setattr(self, hk_subform, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_subform, 'thisown', 0)
        _swig_setattr(self, hk_subform,self.__class__,hk_subform)
_hk_classes.hk_subform_swigregister(hk_subformPtr)

class hk_url(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_url, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_url, name)
    def __repr__(self):
        return "<C hk_url instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_url, 'this', _hk_classes.new_hk_url(*args))
        _swig_setattr(self, hk_url, 'thisown', 1)
    def is_valid(*args): return _hk_classes.hk_url_is_valid(*args)
    def is_local(*args): return _hk_classes.hk_url_is_local(*args)
    def url(*args): return _hk_classes.hk_url_url(*args)
    def filename(*args): return _hk_classes.hk_url_filename(*args)
    def directory(*args): return _hk_classes.hk_url_directory(*args)
    def extension(*args): return _hk_classes.hk_url_extension(*args)
    def __del__(self, destroy=_hk_classes.delete_hk_url):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_urlPtr(hk_url):
    def __init__(self, this):
        _swig_setattr(self, hk_url, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_url, 'thisown', 0)
        _swig_setattr(self, hk_url,self.__class__,hk_url)
_hk_classes.hk_url_swigregister(hk_urlPtr)

class hk_datetime(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_datetime, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_datetime, name)
    def __repr__(self):
        return "<C hk_datetime instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_datetime, 'this', _hk_classes.new_hk_datetime(*args))
        _swig_setattr(self, hk_datetime, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_datetime):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_date(*args): return _hk_classes.hk_datetime_set_date(*args)
    def set_time(*args): return _hk_classes.hk_datetime_set_time(*args)
    def set_datetime(*args): return _hk_classes.hk_datetime_set_datetime(*args)
    def set_date_asstring(*args): return _hk_classes.hk_datetime_set_date_asstring(*args)
    def set_time_asstring(*args): return _hk_classes.hk_datetime_set_time_asstring(*args)
    def set_datetime_asstring(*args): return _hk_classes.hk_datetime_set_datetime_asstring(*args)
    def date_asstring(*args): return _hk_classes.hk_datetime_date_asstring(*args)
    def time_asstring(*args): return _hk_classes.hk_datetime_time_asstring(*args)
    def datetime_asstring(*args): return _hk_classes.hk_datetime_datetime_asstring(*args)
    def set_dateformat(*args): return _hk_classes.hk_datetime_set_dateformat(*args)
    def set_timeformat(*args): return _hk_classes.hk_datetime_set_timeformat(*args)
    def set_datetimeformat(*args): return _hk_classes.hk_datetime_set_datetimeformat(*args)
    def day(*args): return _hk_classes.hk_datetime_day(*args)
    def month(*args): return _hk_classes.hk_datetime_month(*args)
    def year(*args): return _hk_classes.hk_datetime_year(*args)
    def hour(*args): return _hk_classes.hk_datetime_hour(*args)
    def minute(*args): return _hk_classes.hk_datetime_minute(*args)
    def second(*args): return _hk_classes.hk_datetime_second(*args)
    def set_now(*args): return _hk_classes.hk_datetime_set_now(*args)
    def __eq__(*args): return _hk_classes.hk_datetime___eq__(*args)
    def __ne__(*args): return _hk_classes.hk_datetime___ne__(*args)
    def __gt__(*args): return _hk_classes.hk_datetime___gt__(*args)
    def __ge__(*args): return _hk_classes.hk_datetime___ge__(*args)
    def __lt__(*args): return _hk_classes.hk_datetime___lt__(*args)
    def __le__(*args): return _hk_classes.hk_datetime___le__(*args)
    def difference(*args): return _hk_classes.hk_datetime_difference(*args)

class hk_datetimePtr(hk_datetime):
    def __init__(self, this):
        _swig_setattr(self, hk_datetime, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_datetime, 'thisown', 0)
        _swig_setattr(self, hk_datetime,self.__class__,hk_datetime)
_hk_classes.hk_datetime_swigregister(hk_datetimePtr)


transfer_date = _hk_classes.transfer_date

transfer_time = _hk_classes.transfer_time

transfer_datetime = _hk_classes.transfer_datetime
class colstruct(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, colstruct, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, colstruct, name)
    def __repr__(self):
        return "<C colstruct instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, colstruct, 'this', _hk_classes.new_colstruct(*args))
        _swig_setattr(self, colstruct, 'thisown', 1)
    __swig_setmethods__["colname"] = _hk_classes.colstruct_colname_set
    __swig_getmethods__["colname"] = _hk_classes.colstruct_colname_get
    if _newclass:colname = property(_hk_classes.colstruct_colname_get, _hk_classes.colstruct_colname_set)
    __swig_setmethods__["col"] = _hk_classes.colstruct_col_set
    __swig_getmethods__["col"] = _hk_classes.colstruct_col_get
    if _newclass:col = property(_hk_classes.colstruct_col_get, _hk_classes.colstruct_col_set)
    __swig_setmethods__["currentvalue"] = _hk_classes.colstruct_currentvalue_set
    __swig_getmethods__["currentvalue"] = _hk_classes.colstruct_currentvalue_get
    if _newclass:currentvalue = property(_hk_classes.colstruct_currentvalue_get, _hk_classes.colstruct_currentvalue_set)
    def __del__(self, destroy=_hk_classes.delete_colstruct):
        try:
            if self.thisown: destroy(self)
        except: pass

class colstructPtr(colstruct):
    def __init__(self, this):
        _swig_setattr(self, colstruct, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, colstruct, 'thisown', 0)
        _swig_setattr(self, colstruct,self.__class__,colstruct)
_hk_classes.colstruct_swigregister(colstructPtr)

class hk_importcsv(hk_dsvisible):
    __swig_setmethods__ = {}
    for _s in [hk_dsvisible]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_importcsv, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_dsvisible]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_importcsv, name)
    def __repr__(self):
        return "<C hk_importcsv instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_importcsv, 'this', _hk_classes.new_hk_importcsv(*args))
        _swig_setattr(self, hk_importcsv, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_importcsv):
        try:
            if self.thisown: destroy(self)
        except: pass
    def execute(*args): return _hk_classes.hk_importcsv_execute(*args)
    def set_filedefinition(*args): return _hk_classes.hk_importcsv_set_filedefinition(*args)
    def set_filename(*args): return _hk_classes.hk_importcsv_set_filename(*args)
    def set_firstrow_contains_fieldnames(*args): return _hk_classes.hk_importcsv_set_firstrow_contains_fieldnames(*args)
    def set_append_rows(*args): return _hk_classes.hk_importcsv_set_append_rows(*args)
    def set_create_new_table(*args): return _hk_classes.hk_importcsv_set_create_new_table(*args)
    def add_columnname(*args): return _hk_classes.hk_importcsv_add_columnname(*args)
    def clear_columnlist(*args): return _hk_classes.hk_importcsv_clear_columnlist(*args)
    def set_detect_autoinc(*args): return _hk_classes.hk_importcsv_set_detect_autoinc(*args)
    def detect_autoinc(*args): return _hk_classes.hk_importcsv_detect_autoinc(*args)
    def textdelimiter(*args): return _hk_classes.hk_importcsv_textdelimiter(*args)
    def betweenfield(*args): return _hk_classes.hk_importcsv_betweenfield(*args)
    def rowdelimiter(*args): return _hk_classes.hk_importcsv_rowdelimiter(*args)
    def datetimeformat(*args): return _hk_classes.hk_importcsv_datetimeformat(*args)
    def dateformat(*args): return _hk_classes.hk_importcsv_dateformat(*args)
    def timeformat(*args): return _hk_classes.hk_importcsv_timeformat(*args)
    def filelocale(*args): return _hk_classes.hk_importcsv_filelocale(*args)
    def filecharset(*args): return _hk_classes.hk_importcsv_filecharset(*args)
    def set_overwrite_table(*args): return _hk_classes.hk_importcsv_set_overwrite_table(*args)
    def overwrite_table(*args): return _hk_classes.hk_importcsv_overwrite_table(*args)
    def create_valuelist(*args): return _hk_classes.hk_importcsv_create_valuelist(*args)
    def get_line(*args): return _hk_classes.hk_importcsv_get_line(*args)

class hk_importcsvPtr(hk_importcsv):
    def __init__(self, this):
        _swig_setattr(self, hk_importcsv, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_importcsv, 'thisown', 0)
        _swig_setattr(self, hk_importcsv,self.__class__,hk_importcsv)
_hk_classes.hk_importcsv_swigregister(hk_importcsvPtr)

class hk_reporthtml(hk_report):
    __swig_setmethods__ = {}
    for _s in [hk_report]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_reporthtml, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_report]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_reporthtml, name)
    def __repr__(self):
        return "<C hk_reporthtml instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_reporthtml, 'this', _hk_classes.new_hk_reporthtml(*args))
        _swig_setattr(self, hk_reporthtml, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_reporthtml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_tabletag(*args): return _hk_classes.hk_reporthtml_set_tabletag(*args)
    def set_beforetable(*args): return _hk_classes.hk_reporthtml_set_beforetable(*args)
    def set_aftertable(*args): return _hk_classes.hk_reporthtml_set_aftertable(*args)
    def set_trtag(*args): return _hk_classes.hk_reporthtml_set_trtag(*args)
    def set_thtag(*args): return _hk_classes.hk_reporthtml_set_thtag(*args)
    def set_tdtag(*args): return _hk_classes.hk_reporthtml_set_tdtag(*args)
    def set_beforehtmltag(*args): return _hk_classes.hk_reporthtml_set_beforehtmltag(*args)
    def set_betweenheadtags(*args): return _hk_classes.hk_reporthtml_set_betweenheadtags(*args)
    def set_groupcolumn(*args): return _hk_classes.hk_reporthtml_set_groupcolumn(*args)
    def set_multiplefiles(*args): return _hk_classes.hk_reporthtml_set_multiplefiles(*args)
    def set_title(*args): return _hk_classes.hk_reporthtml_set_title(*args)
    def set_author(*args): return _hk_classes.hk_reporthtml_set_author(*args)

class hk_reporthtmlPtr(hk_reporthtml):
    def __init__(self, this):
        _swig_setattr(self, hk_reporthtml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_reporthtml, 'thisown', 0)
        _swig_setattr(self, hk_reporthtml,self.__class__,hk_reporthtml)
_hk_classes.hk_reporthtml_swigregister(hk_reporthtmlPtr)


recode_html = _hk_classes.recode_html

recode_postscript = _hk_classes.recode_postscript

fullpagereplace_postscript = _hk_classes.fullpagereplace_postscript

recode_utf8 = _hk_classes.recode_utf8

recount_postscript = _hk_classes.recount_postscript

configure_postscriptdata = _hk_classes.configure_postscriptdata

configure_postscriptreport = _hk_classes.configure_postscriptreport

set_reporttype_postscript = _hk_classes.set_reporttype_postscript

replacepsdatafunction = _hk_classes.replacepsdatafunction
class hk_reportcsv(hk_report):
    __swig_setmethods__ = {}
    for _s in [hk_report]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_reportcsv, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_report]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_reportcsv, name)
    def __repr__(self):
        return "<C hk_reportcsv instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_reportcsv, 'this', _hk_classes.new_hk_reportcsv(*args))
        _swig_setattr(self, hk_reportcsv, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_reportcsv):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_groupcolumn(*args): return _hk_classes.hk_reportcsv_set_groupcolumn(*args)
    def set_multiplefiles(*args): return _hk_classes.hk_reportcsv_set_multiplefiles(*args)
    def set_filedefinition(*args): return _hk_classes.hk_reportcsv_set_filedefinition(*args)
    def set_withheader(*args): return _hk_classes.hk_reportcsv_set_withheader(*args)
    def textdelimiter(*args): return _hk_classes.hk_reportcsv_textdelimiter(*args)
    def between(*args): return _hk_classes.hk_reportcsv_between(*args)

class hk_reportcsvPtr(hk_reportcsv):
    def __init__(self, this):
        _swig_setattr(self, hk_reportcsv, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_reportcsv, 'thisown', 0)
        _swig_setattr(self, hk_reportcsv,self.__class__,hk_reportcsv)
_hk_classes.hk_reportcsv_swigregister(hk_reportcsvPtr)


encodefilecharsetfunction = _hk_classes.encodefilecharsetfunction

automatic_create_csvfields = _hk_classes.automatic_create_csvfields

zmm2ps = _hk_classes.zmm2ps

ps2zmm = _hk_classes.ps2zmm
class hk_reportxml(hk_report):
    __swig_setmethods__ = {}
    for _s in [hk_report]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_reportxml, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_report]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_reportxml, name)
    def __repr__(self):
        return "<C hk_reportxml instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_reportxml, 'this', _hk_classes.new_hk_reportxml(*args))
        _swig_setattr(self, hk_reportxml, 'thisown', 1)
    def __del__(self, destroy=_hk_classes.delete_hk_reportxml):
        try:
            if self.thisown: destroy(self)
        except: pass
    def set_maindocumenttag(*args): return _hk_classes.hk_reportxml_set_maindocumenttag(*args)
    def maindocumenttag(*args): return _hk_classes.hk_reportxml_maindocumenttag(*args)
    def set_rowtag(*args): return _hk_classes.hk_reportxml_set_rowtag(*args)
    def rowtag(*args): return _hk_classes.hk_reportxml_rowtag(*args)
    def set_includetableschema(*args): return _hk_classes.hk_reportxml_set_includetableschema(*args)
    def set_excelxml(*args): return _hk_classes.hk_reportxml_set_excelxml(*args)
    def excelxml(*args): return _hk_classes.hk_reportxml_excelxml(*args)
    def includetableschema(*args): return _hk_classes.hk_reportxml_includetableschema(*args)
    fieldname = _hk_classes.hk_reportxml_fieldname
    fieldnameattribute = _hk_classes.hk_reportxml_fieldnameattribute
    def set_fieldname_as_attribute(*args): return _hk_classes.hk_reportxml_set_fieldname_as_attribute(*args)
    def fieldname_as_attribute(*args): return _hk_classes.hk_reportxml_fieldname_as_attribute(*args)

class hk_reportxmlPtr(hk_reportxml):
    def __init__(self, this):
        _swig_setattr(self, hk_reportxml, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_reportxml, 'thisown', 0)
        _swig_setattr(self, hk_reportxml,self.__class__,hk_reportxml)
_hk_classes.hk_reportxml_swigregister(hk_reportxmlPtr)


fullpagereplace_excelxml = _hk_classes.fullpagereplace_excelxml

replaceexcelxmldatafunction = _hk_classes.replaceexcelxmldatafunction

configure_excelxmldata = _hk_classes.configure_excelxmldata
class hk_mimetype(hk_class):
    __swig_setmethods__ = {}
    for _s in [hk_class]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, hk_mimetype, name, value)
    __swig_getmethods__ = {}
    for _s in [hk_class]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, hk_mimetype, name)
    def __repr__(self):
        return "<C hk_mimetype instance at %s>" % (self.this,)
    def __init__(self, *args):
        _swig_setattr(self, hk_mimetype, 'this', _hk_classes.new_hk_mimetype(*args))
        _swig_setattr(self, hk_mimetype, 'thisown', 1)
    __swig_getmethods__["mimetype"] = lambda x: _hk_classes.hk_mimetype_mimetype
    if _newclass:mimetype = staticmethod(_hk_classes.hk_mimetype_mimetype)
    __swig_getmethods__["mimetype"] = lambda x: _hk_classes.hk_mimetype_mimetype
    if _newclass:mimetype = staticmethod(_hk_classes.hk_mimetype_mimetype)
    __swig_getmethods__["set_mimetypedatabasefile"] = lambda x: _hk_classes.hk_mimetype_set_mimetypedatabasefile
    if _newclass:set_mimetypedatabasefile = staticmethod(_hk_classes.hk_mimetype_set_mimetypedatabasefile)
    __swig_getmethods__["mimetypedatabasefile"] = lambda x: _hk_classes.hk_mimetype_mimetypedatabasefile
    if _newclass:mimetypedatabasefile = staticmethod(_hk_classes.hk_mimetype_mimetypedatabasefile)
    def __del__(self, destroy=_hk_classes.delete_hk_mimetype):
        try:
            if self.thisown: destroy(self)
        except: pass

class hk_mimetypePtr(hk_mimetype):
    def __init__(self, this):
        _swig_setattr(self, hk_mimetype, 'this', this)
        if not hasattr(self,"thisown"): _swig_setattr(self, hk_mimetype, 'thisown', 0)
        _swig_setattr(self, hk_mimetype,self.__class__,hk_mimetype)
_hk_classes.hk_mimetype_swigregister(hk_mimetypePtr)

hk_mimetype_mimetype = _hk_classes.hk_mimetype_mimetype

hk_mimetype_set_mimetypedatabasefile = _hk_classes.hk_mimetype_set_mimetypedatabasefile

hk_mimetype_mimetypedatabasefile = _hk_classes.hk_mimetype_mimetypedatabasefile

from os.path import *
def __new_connection(drivermanager,drivername):
	l=drivermanager.driverlist()
	if not  drivername in l:
		reason="No such driver:'"+drivername+"'"
		print reason
		return None
	con=drivermanager.new_connection(drivername)
	if con == None:
		reason= "Connection was not possible"
		print reason
		return None
	con.connect(hk_class.noninteractive)
	if not con.is_connected():
		reason="Connection can't connect:"+con.last_servermessage()
		print reason
		del con
		return None
	return con


def create_database(con,db,deleteexisting=False):
	global reason
	if not con.server_supports(hk_connection.SUPPORTS_NEW_DATABASE):
		reason="Server does not support creating new databases"
		print reason
		return False
	if (db in con.dblist()) and deleteexisting:
		con.delete_database(db,hk_class.noninteractive)
	result=con.create_database(db)
	if result ==1:
		return True
	else:
		reason=con.last_servermessage()
		print reason
		return False

def open_database(drivermanager,drivername,dbname,create=False,deleteexistingdb=False,user="root",password="",host=""):
	try:
		con=__new_connection(drivermanager,drivername)
	except Exception,e:
		print "Exception in newconnection",e
		return None
		
	if con==None:
		print "con==NonE"
		return None
	if create and (not dbname in con.dblist() and not exists(dbname)):
		result=create_database(con,dbname,deleteexistingdb)
		if result!=True:
			reason="create_database failed: "+con.last_servermessage()
			print reason
			return result
	
	if (not dbname in con.dblist() and not exists(dbname)):
		reason="database '"+dbname+"' does not exist"
		print reason
		
		return None
	db=con.new_database(dbname)
	if db==None:
		reason= "Database could not be created: "+con.last_servermessage()
		print reason
		return None
	return db


def create_table(db,tbl,coldef,values,valuecolumns,insert=False):
	global reason
	if not db.connection().server_supports(hk_connection.SUPPORTS_NEW_TABLE):
		reason="Server does not support creating new tables"
		print reason
		return None
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



