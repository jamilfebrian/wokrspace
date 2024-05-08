const url = "https://script.google.com/macros/s/AKfycbxODJDO3sLrVf88WVyQ-0qUzK-tHvrN3_8i9sBpGtiQCDai6iymN9ZPb0xuGTlkqg/exec?sts=read"
//sts=write&srs=Succes&V=11&b2=ON&b1=OFF&b3=OFF

// var fullURL = `  ${}`









fetch(url)
  .then(response => {
    if (!response.ok) {
        throw new Error('Network response was not ok ' + response.statusText);
    }
    return response.text(); // Menguraikan respons menjadi JSON
  })
  .then(data => {
    console.log(data); // Menampilkan data yang diterima dari server
  })
  .catch(error => {
    console.error('There was a problem with the fetch operation:', error);
  });
