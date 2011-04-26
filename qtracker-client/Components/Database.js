//Database.js

function getDatabase() {
     return openDatabaseSync("qTracker", "1.0", "StorageDatabase", 100000);
}

function setup() {
    var db = getDatabase();
    db.transaction(
        function(tx) {
            // Create tables if they don't exist yet
            tx.executeSql('CREATE TABLE IF NOT EXISTS settings    (setting TEXT UNIQUE, value TEXT)');
            tx.executeSql('CREATE TABLE IF NOT EXISTS tracks      (trackid INTEGER PRIMARY KEY, name TEXT, interval INTEGER, top REAL, left REAL, bottom REAL, right REAL);');
            tx.executeSql('CREATE TABLE IF NOT EXISTS trackpoints (pointid INTEGER PRIMARY KEY, trackid INTEGER, latitude REAL, longitude REAL, altitude REAL)')
          });
}

// This function is used to write a setting into the database
function setSetting(setting, value) {
   // setting: string representing the setting name (eg: “username”)
   // value: string representing the value of the setting (eg: “myUsername”)
   var db = getDatabase();
   var res = "";
   db.transaction(function(tx) {
        var rs = tx.executeSql('INSERT OR REPLACE INTO settings VALUES (?,?);', [setting,value]);
              //console.log(rs.rowsAffected)
              if (rs.rowsAffected > 0) {
                res = "OK";
              } else {
                res = "Error";
              }
        }
  );
  // The function returns “OK” if it was successful, or “Error” if it wasn't
  return res;
}
// This function is used to retrieve a setting from the database
function getSetting(setting) {
   var db = getDatabase();
   var res="";
   db.transaction(function(tx) {
     var rs = tx.executeSql('SELECT value FROM settings WHERE setting=?;', [setting]);
     if (rs.rows.length > 0) {
          res = rs.rows.item(0).value;
     } else {
         res = "Unknown";
     }
  })
  // The function returns “Unknown” if the setting was not found in the database
  // For more advanced projects, this should probably be handled through error codes
  return res
}
