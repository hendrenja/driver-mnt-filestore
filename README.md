# filestore
Store and retrieve objects in/from JSON files

# Usage
The easiest way to add a filestore to your project is to add a configuration file to your project and then load this file with `corto_load` in your application. Here's an example:

`filestore.json`
```javascript
{
  "type": "driver/mnt/filestore",
  "value": {
    "storedir": "/tmp/myProjectStore",
    "query": {
      "from": "data"
    }
  }
}
```

You can then load this file in your application, by doing:
```c
if (corto_load("filestore.json", 0, NULL)) {
    corto_error("%s", corto_lasterr());
}
```

If the file loaded successfully, all objects under `data` will now be persisted to JSON files.
