import 'package:flutter/material.dart';
import 'pages/login_page.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'firebase_options.dart';
// import 'package:test/room_info/room_info.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  FirebaseApp? firebaseApp;
  DatabaseReference ref;

  /********** INIT FIREBASE ***********/
  try {
    firebaseApp = await Firebase.initializeApp(
      options: DefaultFirebaseOptions.currentPlatform,
    );
    ref = FirebaseDatabase.instance.ref();
  } catch (e) {
    print('Error initializing Firebase: $e');
  }

  /********** CHECK FIREBASE INIT & START APP ***********/
  if (firebaseApp != null) {
    print('Firebase App Name: ${firebaseApp.name}');
    runApp(const MyApp());
  } else {
    print('Failed to initialize Firebase');
    // Handle the error - maybe show an error message to the user
  }

  /********** ANONYMOUS SIGN IN TO FIREBASE ***********/
  try {
    final userCredential = await FirebaseAuth.instance.signInAnonymously();
    print("Signed in with temporary account.");
  } on FirebaseAuthException catch (e) {
    switch (e.code) {
      case "operation-not-allowed":
        print("Anonymous auth hasn't been enabled for this project.");
        break;
      default:
        print("Unknown error.");
    }
  }
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Smart Home',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const LoginPage(),
    );
  }
}
//flutter run -d web-server  to run on brave or any browser if it is not shown on Devices down there