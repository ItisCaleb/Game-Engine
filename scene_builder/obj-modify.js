document.querySelector('#obj-x').oninput = ()=>{
    let s = canvas.getActiveObject()
    let target = Number(document.querySelector('#obj-x').value);
    if(s) {
        s.left = target;
        canvas.fire('object:modified');
    }
  }
  
  document.querySelector('#obj-y').oninput = ()=>{
    let s = canvas.getActiveObject()
    let target = Number(document.querySelector('#obj-y').value);
    if(s) {
        s.top = target;
        canvas.fire('object:modified');
    }
  }
  document.querySelector('#obj-width').oninput = ()=>{
    let s = canvas.getActiveObject()
    let target = Number(document.querySelector('#obj-width').value);
    if(s) {
      s.scaleX = target / s.width;
      canvas.fire('object:modified');
    }
  }
  
  document.querySelector('#obj-height').oninput = ()=>{
    let s = canvas.getActiveObject()
    let target = Number(document.querySelector('#obj-height').value);
    if(s) {
      s.scaleY = target / s.height;
      canvas.fire('object:modified');
    }
  }
  
  canvas.on('object:modified', ()=>{
    let s = canvas.getActiveObject()
    document.querySelector('#obj-x').value = s.left;
    document.querySelector('#obj-y').value = s.top;
    document.querySelector('#obj-width').value = s.width * s.scaleX;
    document.querySelector('#obj-height').value = s.height * s.scaleY;
    canvas.renderAll();
    output_json();
  });
  
  canvas.on('selection:created', ()=>{
    let s = canvas.getActiveObject()
    document.querySelector('#obj-x').value = s.left;
    document.querySelector('#obj-y').value = s.top;
    document.querySelector('#obj-width').value = s.width * s.scaleX;
    document.querySelector('#obj-height').value = s.height * s.scaleY;
  });
  
  canvas.on('selection:cleared', ()=> {
    document.querySelector('#obj-x').value = 0
    document.querySelector('#obj-y').value = 0
    document.querySelector('#obj-width').value = 0
    document.querySelector('#obj-height').value = 0
  });
  