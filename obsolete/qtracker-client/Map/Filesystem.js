
function base(filename) {
    var txt = String(filename);
    var p1 = txt.lastIndexOf('/');
    var p2 = txt.lastIndexOf('.');
    return txt.slice(p1+1,p2);
}

function path(filename) {
    var txt = String(filename);
    var p = txt.lastIndexOf('/');
    return txt.slice(0,p);
}

function ext(filename) {
    var txt = String(filename);
    var p = txt.lastIndexOf('.');
    return txt.slice(p+1);
}
