// jshint esversion:6

const express = require("express");
const bodyParser = require("body-parser");

const app = express();

app.use(express.static("public"));
app.use(bodyParser.urlencoded({ extended: true}));

app.get("/",function(req,res){
  res.sendFile(__dirname + "/signin.html");
})

app.post("/",function(req,res){
  var email= req.body.emaill;
  var password= req.body.pw;
  console.log(email,password);

  if(email=== "110117056@nitt.edu" && password==="abc"){res.sendFile(__dirname + "/items.html")}
})

app.post("/items.html",function(req,res){res.sendFile(__dirname + "/index.html");})

app.post("/index.html",function(req,res){
  res.redirect("/");
})
app.listen(process.env.PORT || 3000,function(){console.log("Working");})
