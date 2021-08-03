class db_kernel
    import sqlite3

    DB_PATH = 'dat.db'

    @staticmethod
    exec_with func -> list
        import os.path
        db_kernel.init_db unless os.path.exists db_kernel.DB_PATH

        con = db_kernel.sqlite3.connect db_kernel.DB_PATH
        with con
            ret = list . func con
        con.close

        return ret

class db <- db_kernel
    '''wrappers'''
    @staticmethod
    __call__ script: str, parameter=None -> [tuple]
            = db.exec_with func
        where parameter
                | isinstance(parameter, tuple)
                        = [ parameter ]
                | otherwise = parameter
              script
                import re
                | INSERT_PATTERN.match script.strip()
                     = f'insert into {script} values ({question_marks})'
                | otherwise = script
                where \
                  INSERT_PATTERN = re.compile '^\S+\(\s*(\S+,\s*)*\S+\s*\)$'
                  question_marks = ', '.join([ '?' ] * len(parameter[0]))

              func con
                | parameter = con.executemany(script, parameter)
                | otherwise = con.execute(script)
