
function twoDigits(d) {
    if(0 <= d && d < 10) return "0" + d.toString();
    if(-10 < d && d < 0) return "-0" + (-1*d).toString();
    return d.toString();
}

Date.toMysqlFormat = function(date) {
    return date.getFullYear() + "-" + twoDigits(1 + date.getMonth()) + "-" + twoDigits(date.getDate()) + "T" + twoDigits(date.getHours()) + ":" + twoDigits(date.getMinutes()) + ":" + twoDigits(date.getSeconds());
};

Date.setHoursAndMinutes = function(date, h, m){
	date = new Date(date);
    date.setHours(h);
	date.setMinutes(m);
    return date;
}

Date.createFromMysql = function(mysql_string)
{ 
   if(typeof mysql_string === 'string')
   {
      var t = mysql_string.split(/[- : T]/);
      
      // when t[3], t[4] and t[5] are missing they defaults to zero
      return new Date(t[0], t[1] - 1, t[2], t[3] || 0, t[4] || 0, t[5] || 0);  
   }

   return null;   
}

function isInArray(value, array) {
	return array.indexOf(value) > -1;
}