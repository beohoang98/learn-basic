document.addEventListener('DOMContentLoaded', async ()=>{
    const img_wrapper = document.querySelector('#img');
    for (let i = 0; i < 6; ++i) {
        const img = new Image(100);
        img.src = "/static/images/cat" + i + ".jpeg";

        img_wrapper.append(img);
    }
});
