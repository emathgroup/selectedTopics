---
title: "Basic Understanding of File Api"
date: "2019-07-25"
thumbnail: "../thumbnails/bash.png"
tags:
    - "javascript"
---

 One of my favourite and exciting topic was how can i implement file uploading in my website. I used to watch youtube videos where they do it with `PHP`. But they all just don't workout for me. Thank god there's `File API` which  allows javascript to access information about file like `Name`, `Last modified`, `Type`, `Size`, `webkitRelativePath`. Let's dive deeper on how to use it.
 
 ```HTML
 <!--  to get file as input -->
<input type="file">

 ```
 Initial input value would be zero and as the user selects file it changes.
 ```js
//  selecting element
const el = document.querySelector('input[type=file]')
//  fire an event handler when the input value changes
    el.addEventListener('change',function(){
    const fileList = el.files; /* now you can work with the file list */    
})
 ```
 Each file object provides three useful information about file.
 - `Name` : The file's name as a read-only string. This is just the file name, and does not include any path information. 
 - `Size` : The size of the file in bytes as a read-only 64-bit integer.
 - `Type` : The MIME type of the file as a read-only string or `""` if the type couldn't be determined.

Until Now we have got the filelist selected by user. let's do some operations on this.
 ### Getting Information about selected files
 ---
 Let's say we want an image less than 5MB. To allow multiplle files we can  add multiple attribute to input tag.
 ```HTML
 <input type="file" multiple>
 ```
 It will give us all object in filelist object.
 ```js
// using foreach loop to iterate through all object
// 5 Mb have 5*1024 byte
fileList.forEach((file)=>{
    // taking each file as file variable
if(file.size <= 5*1024 && file.type == "image/jpeg"){
    // do something
}
else{
    alert("File size Excedded")
}
})
 ```
Above I have used ES6 classes. If You are not comfortable with that. Don't worry. I have simplified it further.
```js
fileList.forEach(function(file){
    // referring each file as file variable
if(file.size <= 5*1024 && file.type="image/jpeg"){
    // do something
}
else{
    alert("File size excedded.")
}
})
```



