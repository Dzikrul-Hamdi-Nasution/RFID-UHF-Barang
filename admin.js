var config = {
    apiKey: "AIzaSyDdALMV0tinjyVRZNv0eT_wjaK26lnw5Fw",
    authDomain: "rfid-uhf-logger.firebaseapp.com",
    databaseURL: "https://rfid-uhf-logger-default-rtdb.firebaseio.com",
    projectId: "rfid-uhf-logger",
    storageBucket: "rfid-uhf-logger.appspot.com",
    messagingSenderId: "174286425345",
    appId: "1:174286425345:web:0339f199c3e636e94d9f00",
    measurementId: "G-XTL4MP9RD8"
  };
  firebase.initializeApp(config);
  
  
  
  $(document).ready(function() {
    

    RFID_data();

    firebase.database().ref("mode").update({
        nilai: "input",
     });
    
  });
  
 
  
  function RFID_data() {
    var messagesRef = firebase.database().ref("scan_rfid");
    messagesRef.on('value', (snapshot) => {
        const data = snapshot.val();
        document.getElementById("rfid_scan").innerHTML =data;
        //$("#rfid_scan").val(data);
    });
    
  }


  function okay() {
    data_rfid="";
    jumlah_aset="";
    jenis_barang = document.getElementById("jenis_barang").value;
    var messagesRef = firebase.database().ref("scan_rfid");
    messagesRef.on('value', (snapshot) => {
        const data = snapshot.val();
        data_rfid=data;
    });

    firebase.database().ref("List/"+data_rfid).update({
        jenis: jenis_barang,
        nilai: data_rfid,
     });

     
    var messagesRef = firebase.database().ref("ID/"+jenis_barang+"/jumlah");
    messagesRef.on('value', (snapshot) => {
        const data = snapshot.val();
        jumlah_aset=data;
    });

    if(jumlah_aset==null){
        firebase.database().ref("ID/"+jenis_barang).update({
        jumlah: 1,
        nama: jenis_barang,
        });
        alert("jumlah aset "+jenis_barang+" = "+jumlah_aset )
    }
    else{
        jumlah_aset=parseInt(jumlah_aset)+1;
        firebase.database().ref("ID/"+jenis_barang).update({
            jumlah: jumlah_aset,
            nama: jenis_barang,
            });
        alert("jumlah aset "+jenis_barang+" = "+jumlah_aset )
    }

   
    //alert("Inventaris berhasil ditambahkan");

  
  
  }
  



  var dbRef = firebase.database();
  var statusAlat = dbRef.ref("List");
   
  // Dapatkan referensi table
  var table = document.getElementById("tabel-status-alat").getElementsByTagName('tbody')[0];;
   
  var total_harga=0,nomor=0;
  // Memuat Data
  statusAlat.on("child_added", function(data, prevChildKey) {
    nomor++
  var newstatusAlat = data.val();
   
  var row = table.insertRow(table.rows.length);
   
  var cell1 = row.insertCell(0);
  var cell2 = row.insertCell(1);
 
  
  
  cell2.innerHTML = newstatusAlat.jenis;
  cell1.innerHTML = newstatusAlat.nilai;
  
  });
  
  function hapus() {
    let userRef1 = firebase.database().ref("List");
    userRef1.remove()
    let userRef = firebase.database().ref("ID");
    userRef.remove()
    alert("Data berhasil dihapus, Silahkan Reload halaman")
  }
  
  
   
    
  
  
  
  
  
  
  
  
  
  
  