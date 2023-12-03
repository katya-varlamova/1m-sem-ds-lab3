\c tickets
INSERT INTO ticket (ticket_uid ,username, flight_number, price, status) VALUES
('049161bb-badd-4fa8-9d90-87c9a82b0668', 'Test Max', 'AFL031', 1500, 'PAID');

\c flights
INSERT INTO airport (name, city, country) VALUES
('Шереметьево', 'Москва', 'Россия');
INSERT INTO airport (name, city, country) VALUES
('Пулково', 'Санкт-Петербург', 'Россия');
INSERT INTO flight (flight_number, datetime, from_airport_id, to_airport_id, price) VALUES
('AFL031', '2021-10-08 20:00', 2, 1, 1500);


\c privileges
INSERT INTO privilege (username, status, balance) VALUES
('Test Max', 'GOLD', 1500);
INSERT INTO privilege_history (privilege_id, ticket_uid, datetime, balance_diff, operation_type) VALUES
(1,
 '049161bb-badd-4fa8-9d90-87c9a82b0668',
 '2021-10-08T19:59:19Z',
 1500,
 'FILL_IN_BALANCE');