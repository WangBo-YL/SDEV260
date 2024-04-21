--Test data for budget table creation.

CREATE TABLE IF NOT EXISTS budget(
	budget_id integer PRIMARY KEY,
	budget_name varchar(30) NOT NULL,
	total numeric(6,3) NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS item(
	item_id integer PRIMARY KEY,
	item_name varchar(30) NOT NULL,
	limit numeric(6,3) NOT NULL DEFAULT 0,
	total numeric(6,3) NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS category(
	cat_name varchar(30) PRIMARY KEY,
	total numeric(6,3) NOT NULL DEFAULT 0,
	budget_id integer NOT NULL REFERENCES budget(budget_id)
);

CREATE TABLE IF NOT EXISTS saving(
	sav_id integer PRIMARY KEY,
	sav_name varchar(30) NOT NULL,
	goal numeric(6,3) NOT NULL DEFAULT 0,
	total numeric(6,3) NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS transactions(
	trans_id integer PRIMARY KEY,
	total numeric NOT NULL,
	item_id integer REFERENCES item(item_id), 
	sav_id integer REFERENCES saving(sav_id),
	cat_name varchar(30) REFERENCES category(cat_name)
);