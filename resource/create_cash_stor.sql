----------------------------------------------------------------------------
--
-- Script to create cash data storage.
--
-- (C) T&T team, Kiev, Ukraine 2016.<br>
-- started 22.01.2016  9:32:22<br>
-- @author oleg
-- @version 0.01
--
------------------------------------------------------------------------------
-- !!! Delimiter of statements is +++ !!!!

CREATE TABLE IF NOT EXISTS PLU_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
C INTEGER,
CHANGES TIMESTAMP
);
+++
CREATE TABLE IF NOT EXISTS PLU(
C INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
NM TEXT,
GRP INTEGER,
DPT INTEGER,
TX INTEGER,
TX2 INTEGER,
"DEC" INTEGER
);
+++
CREATE TRIGGER IF NOT EXISTS PLU_INSERT AFTER INSERT ON PLU FOR EACH ROW
BEGIN
  INSERT INTO PLU_REV(C,CHANGES) VALUES(NEW.C, DATETIME('now'));
  UPDATE PLU SET REV=LAST_INSERT_ROWID() WHERE C = NEW.C;
END;
+++
CREATE TRIGGER IF NOT EXISTS PLU_UPDATE AFTER UPDATE OF DEL,NM,GRP,DPT,TX,TX2,"DEC" ON PLU FOR EACH ROW
BEGIN
  INSERT INTO PLU_REV(C,CHANGES)
    SELECT NEW.C, DATETIME('now') WHERE
            OLD.DEL <> NEW.DEL
        OR  OLD.NM <> NEW.NM
        OR  OLD.GRP <> NEW.GRP
        OR  OLD.DPT <> NEW.DPT
        OR  OLD.TX <> NEW.TX
        OR  OLD.TX2 <> NEW.TX2
        OR  OLD."DEC" <> NEW."DEC"
        ;

    UPDATE PLU SET REV=(
    CASE WHEN
        (OLD.DEL <> NEW.DEL
        OR  OLD.NM <> NEW.NM
        OR  OLD.GRP <> NEW.GRP
        OR  OLD.DPT <> NEW.DPT
        OR  OLD.TX <> NEW.TX
        OR  OLD.TX2 <> NEW.TX2
        OR  OLD."DEC" <> NEW."DEC"
        )
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE C = NEW.C
    ;
END;
+++
------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS PRC_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
C INTEGER,
CHANGES TIMESTAMP
);
+++
CREATE TABLE IF NOT EXISTS PRC(
C INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
PRC DOUBLE
);
+++
CREATE TRIGGER IF NOT EXISTS PRC_INSERT AFTER INSERT ON PRC FOR EACH ROW
BEGIN
  INSERT INTO PRC_REV(C,CHANGES) VALUES(NEW.C, DATETIME('now'));
  UPDATE PRC SET REV=LAST_INSERT_ROWID() WHERE C = NEW.C;
END;
+++
CREATE TRIGGER IF NOT EXISTS PRC_UPDATE AFTER UPDATE OF DEL, PRC ON PRC FOR EACH ROW
BEGIN
  INSERT INTO PRC_REV(C,CHANGES)
    SELECT NEW.C, DATETIME('now') WHERE OLD.DEL <> NEW.DEL OR OLD.PRC <> NEW.PRC;

    UPDATE PRC SET REV=(
    CASE WHEN (OLD.DEL <> NEW.DEL OR OLD.PRC <> NEW.PRC)
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE C = NEW.C
    ;
END;
------------------------------------------------------------------------------*/
+++
CREATE TABLE IF NOT EXISTS GRP_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
C INTEGER,
CHANGES TIMESTAMP
);
+++
CREATE TABLE IF NOT EXISTS GRP(
C INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
NM TEXT
);
+++

CREATE TRIGGER IF NOT EXISTS GRP_INSERT AFTER INSERT ON GRP FOR EACH ROW
BEGIN
  INSERT INTO GRP_REV(C,CHANGES) VALUES(NEW.C, DATETIME('now'));
  UPDATE GRP SET REV=LAST_INSERT_ROWID() WHERE C = NEW.C;
END;
+++

CREATE TRIGGER IF NOT EXISTS GRP_UPDATE AFTER UPDATE OF DEL, NM ON GRP FOR EACH ROW
BEGIN
  INSERT INTO GRP_REV(C,CHANGES)
    SELECT NEW.C, DATETIME('now') WHERE OLD.DEL <> NEW.DEL OR OLD.NM <> NEW.NM;

    UPDATE GRP SET REV=(
    CASE WHEN (OLD.DEL <> NEW.DEL OR OLD.NM <> NEW.NM)
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE C = NEW.C
    ;
END;
------------------------------------------------------------------------------*/
+++
CREATE TABLE IF NOT EXISTS DPT_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
C INTEGER,
CHANGES TIMESTAMP
);

+++
CREATE TABLE IF NOT EXISTS DPT(
C INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
NM TEXT
);


+++
CREATE TRIGGER IF NOT EXISTS DPT_INSERT AFTER INSERT ON DPT FOR EACH ROW
BEGIN
  INSERT INTO DPT_REV(C,CHANGES) VALUES(NEW.C, DATETIME('now'));
  UPDATE DPT SET REV=LAST_INSERT_ROWID() WHERE C = NEW.C;
END;

+++
CREATE TRIGGER IF NOT EXISTS DPT_UPDATE AFTER UPDATE OF NM, DEL ON DPT FOR EACH ROW
BEGIN
  INSERT INTO DPT_REV(C,CHANGES)
    SELECT NEW.C, DATETIME('now') WHERE OLD.DEL <> NEW.DEL OR OLD.NM <> NEW.NM;

    UPDATE DPT SET REV=(
    CASE WHEN (OLD.DEL <> NEW.DEL OR OLD.NM <> NEW.NM)
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE C = NEW.C
    ;
END;
------------------------------------------------------------------------------*/
+++
CREATE TABLE IF NOT EXISTS BAR_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
ID INTEGER,
CHANGES TIMESTAMP
);

+++
CREATE TABLE IF NOT EXISTS BAR(
ID INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
CD TEXT,
C INTEGER
);

+++
CREATE INDEX IF NOT EXISTS BAR_IDX1 ON BAR(CD);
+++
CREATE TRIGGER IF NOT EXISTS BAR_INSERT AFTER INSERT ON BAR FOR EACH ROW
BEGIN
  INSERT INTO BAR_REV(ID,CHANGES) VALUES(NEW.ID, DATETIME('now'));
  UPDATE BAR SET REV=LAST_INSERT_ROWID() WHERE ID = NEW.ID;
END;

+++
CREATE TRIGGER IF NOT EXISTS BAR_UPDATE AFTER UPDATE OF ID, DEL, CD, C ON BAR FOR EACH ROW
BEGIN
  INSERT INTO BAR_REV(ID,CHANGES)
    SELECT NEW.ID, DATETIME('now') WHERE
        OLD.DEL <> NEW.DEL
     OR OLD.CD <> NEW.CD
     OR OLD.C <> NEW.C
     ;

    UPDATE BAR SET REV=(
    CASE WHEN (
        OLD.DEL <> NEW.DEL
     OR OLD.CD <> NEW.CD
     OR OLD.C <> NEW.C
     )
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE ID = NEW.ID
    ;
END;
------------------------------------------------------------------------------*/
+++
CREATE TABLE IF NOT EXISTS EMT_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
C INTEGER,
CHANGES TIMESTAMP
);

+++
CREATE TABLE IF NOT EXISTS EMT(
C INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
F INTEGER,
NM TEXT
);


+++
CREATE TRIGGER IF NOT EXISTS EMT_INSERT AFTER INSERT ON EMT FOR EACH ROW
BEGIN
  INSERT INTO EMT_REV(C,CHANGES) VALUES(NEW.C, DATETIME('now'));
  UPDATE EMT SET REV=LAST_INSERT_ROWID() WHERE C = NEW.C;
END;

+++
CREATE TRIGGER IF NOT EXISTS EMT_UPDATE AFTER UPDATE OF DEL,F,NM ON EMT FOR EACH ROW
BEGIN
  INSERT INTO EMT_REV(C,CHANGES)
    SELECT NEW.C, DATETIME('now') WHERE OLD.DEL <> NEW.DEL OR OLD.F <> NEW.F OR OLD.NM <> NEW.NM;

    UPDATE EMT SET REV=(
    CASE WHEN (OLD.DEL <> NEW.DEL OR OLD.F <> NEW.F OR OLD.NM <> NEW.NM)
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE C = NEW.C
    ;
END;
------------------------------------------------------------------------------*/
+++
CREATE TABLE IF NOT EXISTS CNT_REV(
ROWID INTEGER NOT NULL PRIMARY KEY,
ID INTEGER,
CHANGES TIMESTAMP
);

+++
CREATE TABLE IF NOT EXISTS CNT(
ID INTEGER NOT NULL PRIMARY KEY,
DEL INTEGER NOT NULL DEFAULT 0,
REV INTEGER,
PAN TEXT,
NM TEXT,
EMT INTEGER,
PF INTEGER,
F INTEGER,
PLU INTEGER,
BAL DOUBLE,
PRC DOUBLE
);


+++
CREATE INDEX IF NOT EXISTS CNT_IDX1 ON CNT(PAN);
+++
CREATE TRIGGER IF NOT EXISTS CNT_INSERT AFTER INSERT ON CNT FOR EACH ROW
BEGIN
  INSERT INTO CNT_REV(ID,CHANGES) VALUES(NEW.ID, DATETIME('now'));
  UPDATE CNT SET REV=LAST_INSERT_ROWID() WHERE ID = NEW.ID;
END;

+++
CREATE TRIGGER IF NOT EXISTS CNT_UPDATE AFTER UPDATE OF DEL,PAN,NM,EMT,PF,F,PLU,BAL,PRC ON CNT FOR EACH ROW
BEGIN
  INSERT INTO BAR_REV(ID,CHANGES)
    SELECT NEW.ID, DATETIME('now') WHERE
        OLD.DEL <> NEW.DEL
     OR OLD.PAN <> NEW.PAN
     OR OLD.NM <> NEW.NM
     OR OLD.EMT <> NEW.EMT
     OR OLD.PF <> NEW.PF
     OR OLD.F <> NEW.F
     OR OLD.PLU <> NEW.PLU
     OR OLD.BAL <> NEW.BAL
     OR OLD.PRC <> NEW.PRC
     ;

    UPDATE BAR SET REV=(
    CASE WHEN (
    OLD.DEL <> NEW.DEL
 OR OLD.PAN <> NEW.PAN
 OR OLD.NM <> NEW.NM
 OR OLD.EMT <> NEW.EMT
 OR OLD.PF <> NEW.PF
 OR OLD.F <> NEW.F
 OR OLD.PLU <> NEW.PLU
 OR OLD.BAL <> NEW.BAL
 OR OLD.PRC <> NEW.PRC
    )
        THEN LAST_INSERT_ROWID()
        ELSE OLD.REV
    END)
    WHERE ID = NEW.ID
    ;
END;
------------------------------------------------------------------------------*/
+++
