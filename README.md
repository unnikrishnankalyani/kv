# key-value storage and access system

The following are the main components of the implementation-
  1. A hash-map is used to add keys and access the values
  2. A database is created to persist the key-value pairs
  3. Valid commands are - 
      put: The format is p,key,value, where key is an integer, and value an arbitrary string (without commas in it).
      get: The format is g,key, where key is an integer. 
      delete: The format is d,key, which deletes the relevant key-value pair if present
      clear: The format is c. This command simply removes all key-value pairs from the database.
      all: The format is a. This command prints out all key-value pairs in the database
