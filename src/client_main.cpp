#include <cstdio>

#include <pqxx/pqxx> /* libpqxx is used to instantiate C++ client */


int main(int argc, char **argv) {

  try {
    pqxx::connection C;

    // (
    //   // StringUtil::Format(
    //     "host=127.0.0.1 port=5431 user=postgres sslmode=disable"
    //     // , port
    //   // )
    // );
    // printf("[SimpleQueryTest] Connected to %s\n", C.dbname());
    // pqxx::work txn1(C);

    // txn1.exec("DROP TABLE IF EXISTS employee;");
    // txn1.exec("CREATE TABLE employee(id INT, name VARCHAR(100));");
    // txn1.commit();

    // pqxx::work txn2(C);
    // txn2.exec("INSERT INTO employee VALUES (1, 'Han LI');");
    // txn2.exec("INSERT INTO employee VALUES (2, 'Shaokun ZOU');");
    // txn2.exec("INSERT INTO employee VALUES (3, 'Yilei CHU');");

    // pqxx::result R = txn2.exec("SELECT name FROM employee where id=1;");
    // txn2.commit();

    // EXPECT_EQ(R.size(), 1);
  } catch (const std::exception &e) {
    // LOG_INFO("[SimpleQueryTest] Exception occurred: %s", e.what());
    // EXPECT_TRUE(false);
  }

  return 0;
}








